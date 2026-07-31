//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#include "PhaseFieldElasticityAction.h"

#include "FEProblem.h"
#include "Factory.h"

#include <set>
#include <unordered_map>

registerMooseAction("spfiApp", PhaseFieldElasticityAction, "add_material");

InputParameters
PhaseFieldElasticityAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription(
      "Builds phase-resolved linear elasticity, elastic energies, and optionally an "
      "interpolated global stress independently of a phase-field formulation. This "
      "generic construction supports the elastochemical coupling used by Poudel et al., "
      "Int. J. Solids Struct. 300, 112894 (2024), "
      "doi:10.1016/j.ijsolstr.2024.112894.");

  params.addRequiredParam<std::vector<std::string>>("phase_names", "Unique phase names");
  params.addRequiredParam<std::vector<std::string>>(
      "phase_base_names", "Mechanics material-property base name for each phase");
  params.addRequiredParam<std::vector<std::vector<Real>>>(
      "phase_elasticity_tensors",
      "Elasticity tensor entries arranged as one semicolon-separated row per phase");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "phase_elastic_energies", "Elastic free-energy property generated for each phase");
  params.addRequiredParam<std::vector<VariableName>>(
      "displacements", "Displacement variables used by every strain calculator");

  params.addParam<std::vector<std::vector<Real>>>(
      "phase_eigenstrains",
      {},
      "Optional constant eigenstrain tensor entries, one semicolon-separated row per phase");
  params.addParam<std::vector<MaterialPropertyName>>(
      "phase_eigenstrain_prefactors",
      {},
      "Optional constant values or material properties multiplying each phase eigenstrain");
  params.addParam<std::string>(
      "eigenstrain_name", "eigenstrain", "Eigenstrain property suffix within each phase base");

  params.addParam<std::vector<MaterialPropertyName>>(
      "switching_functions", {}, "Optional switching functions used to interpolate phase stresses");
  params.addParam<std::vector<std::string>>(
      "switching_function_phases", {}, "Phase assigned to each switching function");

  params.addParam<std::string>(
      "elasticity_fill_method", "symmetric9", "RankFourTensor fill method used for all phases");
  params.addParam<std::string>(
      "global_base_name", "global", "Base name of the interpolated global stress");
  params.addParam<bool>(
      "create_global_strain", true, "Create the unprefixed global small-strain material");
  params.addParam<bool>("volumetric_locking_correction",
                        false,
                        "Apply volumetric locking correction in phase and global strains");
  params.addParam<bool>(
      "output_elastic_energies", false, "Output generated phase elastic energies to Exodus");
  params.addParam<std::vector<SubdomainName>>(
      "block", {}, "Optional blocks on which all generated materials are active");

  return params;
}

PhaseFieldElasticityAction::PhaseFieldElasticityAction(const InputParameters & parameters)
  : Action(parameters),
    _phase_names(getParam<std::vector<std::string>>("phase_names")),
    _phase_base_names(getParam<std::vector<std::string>>("phase_base_names")),
    _phase_elasticity_tensors(getParam<std::vector<std::vector<Real>>>("phase_elasticity_tensors")),
    _phase_elastic_energies(getParam<std::vector<MaterialPropertyName>>("phase_elastic_energies")),
    _phase_eigenstrains(getParam<std::vector<std::vector<Real>>>("phase_eigenstrains")),
    _phase_eigenstrain_prefactors(
        getParam<std::vector<MaterialPropertyName>>("phase_eigenstrain_prefactors")),
    _eigenstrain_name(getParam<std::string>("eigenstrain_name")),
    _switching_functions(getParam<std::vector<MaterialPropertyName>>("switching_functions")),
    _switching_function_phases(getParam<std::vector<std::string>>("switching_function_phases")),
    _displacements(getParam<std::vector<VariableName>>("displacements")),
    _elasticity_fill_method(getParam<std::string>("elasticity_fill_method")),
    _global_base_name(getParam<std::string>("global_base_name")),
    _create_global_strain(getParam<bool>("create_global_strain")),
    _volumetric_locking_correction(getParam<bool>("volumetric_locking_correction")),
    _output_elastic_energies(getParam<bool>("output_elastic_energies")),
    _blocks(getParam<std::vector<SubdomainName>>("block"))
{
  const auto phase_count = _phase_names.size();
  if (phase_count == 0)
    paramError("phase_names", "At least one elastic phase is required");
  if (_phase_base_names.size() != phase_count)
    paramError("phase_base_names", "Supply exactly one mechanics base name per phase");
  if (_phase_elasticity_tensors.size() != phase_count)
    paramError("phase_elasticity_tensors", "Supply exactly one elasticity tensor per phase");
  if (_phase_elastic_energies.size() != phase_count)
    paramError("phase_elastic_energies", "Supply exactly one elastic-energy property per phase");
  if (_displacements.empty())
    paramError("displacements", "Supply at least one displacement variable");

  for (const auto & tensor : _phase_elasticity_tensors)
    if (tensor.empty())
      paramError("phase_elasticity_tensors", "Elasticity tensor rows cannot be empty");

  if (!_phase_eigenstrains.empty() && _phase_eigenstrains.size() != phase_count)
    paramError("phase_eigenstrains", "Supply one eigenstrain tensor per phase or omit it");
  for (const auto & eigenstrain : _phase_eigenstrains)
    if (eigenstrain.empty())
      paramError("phase_eigenstrains", "Eigenstrain tensor rows cannot be empty");
  if (!_phase_eigenstrain_prefactors.empty() && _phase_eigenstrains.empty())
    paramError("phase_eigenstrain_prefactors",
               "Set phase_eigenstrains when supplying eigenstrain prefactors");
  if (!_phase_eigenstrain_prefactors.empty() && _phase_eigenstrain_prefactors.size() != phase_count)
    paramError("phase_eigenstrain_prefactors",
               "Supply one eigenstrain prefactor per phase or omit it");

  if (_switching_functions.size() != _switching_function_phases.size())
    paramError("switching_function_phases",
               "Supply one phase assignment for each switching function");

  std::unordered_map<std::string, unsigned int> phase_indices;
  std::set<std::string> base_names;
  std::set<std::string> energy_names;
  for (const auto phase : index_range(_phase_names))
  {
    if (!phase_indices.emplace(_phase_names[phase], phase).second)
      paramError("phase_names", "Phase names must be unique");
    if (!base_names.insert(_phase_base_names[phase]).second)
      paramError("phase_base_names", "Mechanics base names must be unique");
    if (!energy_names.insert(_phase_elastic_energies[phase]).second)
      paramError("phase_elastic_energies", "Elastic-energy property names must be unique");
  }

  _phase_bases_by_switching_function.reserve(_switching_function_phases.size());
  for (const auto & phase_name : _switching_function_phases)
  {
    const auto phase = phase_indices.find(phase_name);
    if (phase == phase_indices.end())
      paramError("switching_function_phases",
                 "Every switching-function phase must be present in phase_names");
    _phase_bases_by_switching_function.push_back(_phase_base_names[phase->second]);
  }
}

void
PhaseFieldElasticityAction::act()
{
  for (const auto phase : index_range(_phase_names))
  {
    {
      InputParameters params = _factory.getValidParams("ComputeElasticityTensor");
      params.set<std::string>("base_name") = _phase_base_names[phase];
      params.set<std::vector<Real>>("C_ijkl") = _phase_elasticity_tensors[phase];
      params.set<MooseEnum>("fill_method") = _elasticity_fill_method;
      applyMaterialScope(params);
      _problem->addMaterial(
          "ComputeElasticityTensor", "spfi_elasticity_tensor_" + _phase_names[phase], params);
    }

    {
      InputParameters params = _factory.getValidParams("ComputeSmallStrain");
      params.set<std::string>("base_name") = _phase_base_names[phase];
      params.set<std::vector<VariableName>>("displacements") = _displacements;
      params.set<bool>("volumetric_locking_correction") = _volumetric_locking_correction;
      if (!_phase_eigenstrains.empty())
        params.set<std::vector<MaterialPropertyName>>("eigenstrain_names") = {_eigenstrain_name};
      applyMaterialScope(params);
      _problem->addMaterial("ComputeSmallStrain", "spfi_strain_" + _phase_names[phase], params);
    }

    {
      InputParameters params = _factory.getValidParams("ComputeLinearElasticStress");
      params.set<std::string>("base_name") = _phase_base_names[phase];
      applyMaterialScope(params);
      _problem->addMaterial(
          "ComputeLinearElasticStress", "spfi_stress_" + _phase_names[phase], params);
    }

    if (!_phase_eigenstrains.empty())
    {
      InputParameters params = _factory.getValidParams("ComputeEigenstrain");
      params.set<std::string>("base_name") = _phase_base_names[phase];
      params.set<std::string>("eigenstrain_name") = _eigenstrain_name;
      params.set<std::vector<Real>>("eigen_base") = _phase_eigenstrains[phase];
      if (!_phase_eigenstrain_prefactors.empty())
        params.set<MaterialPropertyName>("prefactor") = _phase_eigenstrain_prefactors[phase];
      applyMaterialScope(params);
      _problem->addMaterial(
          "ComputeEigenstrain", "spfi_eigenstrain_" + _phase_names[phase], params);
    }

    {
      InputParameters params = _factory.getValidParams("ElasticEnergyMaterial");
      params.set<std::string>("base_name") = _phase_base_names[phase];
      params.set<std::string>("property_name") = _phase_elastic_energies[phase];
      if (_output_elastic_energies)
      {
        params.set<std::vector<std::string>>("output_properties") = {
            _phase_elastic_energies[phase]};
        params.set<std::vector<OutputName>>("outputs") = {"exodus"};
      }
      applyMaterialScope(params);
      _problem->addMaterial(
          "ElasticEnergyMaterial", "spfi_elastic_energy_" + _phase_names[phase], params);
    }
  }

  if (!_switching_functions.empty())
  {
    InputParameters params = _factory.getValidParams("MultiPhaseStressMaterial");
    params.set<std::vector<MaterialPropertyName>>("h") = _switching_functions;
    params.set<std::vector<std::string>>("phase_base") = _phase_bases_by_switching_function;
    if (!_global_base_name.empty())
      params.set<std::string>("base_name") = _global_base_name;
    applyMaterialScope(params);
    _problem->addMaterial("MultiPhaseStressMaterial", "spfi_global_stress", params);
  }

  if (_create_global_strain)
  {
    InputParameters params = _factory.getValidParams("ComputeSmallStrain");
    params.set<std::vector<VariableName>>("displacements") = _displacements;
    params.set<bool>("volumetric_locking_correction") = _volumetric_locking_correction;
    applyMaterialScope(params);
    _problem->addMaterial("ComputeSmallStrain", "spfi_global_strain", params);
  }
}

void
PhaseFieldElasticityAction::applyMaterialScope(InputParameters & params) const
{
  if (!_blocks.empty())
    params.set<std::vector<SubdomainName>>("block") = _blocks;
}
