//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#pragma once

#include "Action.h"

/**
 * Builds phase-resolved small-strain elasticity and interpolated global stress
 * without depending on a particular phase-field free-energy formulation.
 */
class PhaseFieldElasticityAction : public Action
{
public:
  static InputParameters validParams();

  PhaseFieldElasticityAction(const InputParameters & parameters);

  virtual void act() override;

protected:
  void applyMaterialScope(InputParameters & params) const;

  const std::vector<std::string> _phase_names;
  const std::vector<std::string> _phase_base_names;
  const std::vector<std::vector<Real>> _phase_elasticity_tensors;
  const std::vector<MaterialPropertyName> _phase_elastic_energies;

  const std::vector<std::vector<Real>> _phase_eigenstrains;
  const std::vector<MaterialPropertyName> _phase_eigenstrain_prefactors;
  const std::string _eigenstrain_name;

  const std::vector<MaterialPropertyName> _switching_functions;
  const std::vector<std::string> _switching_function_phases;
  std::vector<std::string> _phase_bases_by_switching_function;

  const std::vector<VariableName> _displacements;
  const std::string _elasticity_fill_method;
  const std::string _global_base_name;
  const bool _create_global_strain;
  const bool _volumetric_locking_correction;
  const bool _output_elastic_energies;
  const std::vector<SubdomainName> _blocks;
};
