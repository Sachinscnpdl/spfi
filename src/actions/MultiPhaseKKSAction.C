//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#include "MultiPhaseKKSAction.h"

#include "FEProblem.h"
#include "Factory.h"

#include <algorithm>
#include <set>
#include <unordered_map>

registerMooseAction("spfiApp", MultiPhaseKKSAction, "add_variable");
registerMooseAction("spfiApp", MultiPhaseKKSAction, "add_ic");
registerMooseAction("spfiApp", MultiPhaseKKSAction, "add_material");
registerMooseAction("spfiApp", MultiPhaseKKSAction, "add_kernel");
registerMooseAction("spfiApp", MultiPhaseKKSAction, "add_aux_kernel");

InputParameters
MultiPhaseKKSAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription(
      "Builds a phase-count-agnostic multi-phase KKS system from phase and "
      "order-parameter mappings. The ACGrGrMulti contribution uses the "
      "generalized grain-growth free-energy convention of Moelans, Blanpain, "
      "and Wollants, Phys. Rev. B 78, 024113 (2008), "
      "doi:10.1103/PhysRevB.78.024113.");

  params.addRequiredParam<std::vector<std::string>>("phase_names", "Unique phase names");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "phase_free_energies", "One total free-energy property for each phase");
  params.addRequiredParam<std::vector<std::vector<VariableName>>>(
      "phase_compositions",
      "Phase-composition variables arranged as one "
      "semicolon-separated row per phase");
  params.addRequiredParam<std::vector<VariableName>>("global_compositions",
                                                     "Independent global-composition variables");
  params.addRequiredParam<std::vector<VariableName>>(
      "chemical_potentials", "Chemical-potential variables, one per global composition");
  params.addRequiredParam<std::vector<VariableName>>("order_parameters",
                                                     "All order-parameter variables");
  params.addRequiredParam<std::vector<std::string>>("order_parameter_phases",
                                                    "Phase name assigned to each order parameter");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "switching_functions", "Switching-function property name for each order parameter");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "barrier_functions", "Barrier-function property name for each order parameter");

  params.addRequiredParam<std::string>(
      "kks_reference_phase", "Phase used by the split Cahn-Hilliard chemical-potential kernel");
  params.addParam<std::string>("concentration_constraint_phase",
                               "",
                               "Phase concentration used as the nonlinear "
                               "variable for the concentration constraint; "
                               "the last phase is used when omitted");
  params.addParam<std::vector<VariableName>>(
      "free_energy_coupled_variables",
      {},
      "Variables on which phase free energies depend; phase compositions are "
      "used when omitted");

  params.addParam<std::vector<MaterialPropertyName>>(
      "phase_chemical_free_energies",
      {},
      "Optional chemical free-energy property generated for each phase");
  params.addParam<std::vector<MaterialPropertyName>>(
      "phase_additional_free_energies",
      {},
      "Optional additional contribution, such as elastic energy, for each "
      "phase");
  params.addParam<std::vector<std::string>>(
      "phase_chemical_free_energy_expressions",
      {},
      "Optional arbitrary DerivativeParsedMaterial expression for each phase. "
      "When supplied, "
      "these expressions replace the quadratic free-energy table");
  params.addParam<std::vector<std::string>>(
      "free_energy_material_properties",
      {},
      "Material properties made available by name to every free-energy "
      "expression");
  params.addParam<std::vector<std::string>>(
      "free_energy_constant_names",
      {},
      "Named constants available to every free-energy expression");
  params.addParam<std::vector<std::string>>("free_energy_constant_expressions",
                                            {},
                                            "Values or parsed expressions corresponding to "
                                            "free_energy_constant_names");
  params.addParam<std::vector<VariableName>>(
      "free_energy_expression_coupled_variables",
      {},
      "Additional variables, beyond each phase's composition variables, that "
      "may appear in "
      "free-energy expressions");
  params.addParam<std::vector<Real>>("phase_energy_offsets",
                                     {},
                                     "Legacy convenience mode: constant term "
                                     "of each quadratic chemical free energy");
  params.addParam<std::vector<std::vector<Real>>>(
      "phase_equilibrium_compositions",
      {},
      "Equilibrium compositions arranged as one semicolon-separated row per phase. In "
      "expression mode each value is available as <phase_composition_variable>_eq, and the "
      "same table supplies generated initial compositions when phase_initial_compositions is "
      "omitted");
  params.addParam<std::vector<std::vector<Real>>>(
      "phase_energy_curvatures",
      {},
      "Quadratic curvature for every phase and independent composition");
  params.addParam<Real>(
      "chemical_energy_factor",
      1.0,
      "Common multiplier for generated chemical free energies; it is available by the same "
      "name in phase_chemical_free_energy_expressions");
  params.addParam<MaterialPropertyName>(
      "chemical_length_scale", "length_scale", "Length-scale material property");
  params.addParam<MaterialPropertyName>(
      "chemical_energy_scale", "energy_scale", "Energy-scale material property");
  params.addParam<MaterialPropertyName>(
      "chemical_molar_volume", "molar_vol", "Molar-volume material property");
  params.addParam<bool>("output_generated_free_energies",
                        false,
                        "Output generated chemical and total free-energy properties to Exodus");

  params.addParam<std::vector<VariableName>>(
      "initial_condition_variables",
      {},
      "Variables initialized directly by initial_condition_expressions");
  params.addParam<std::vector<FunctionName>>(
      "initial_condition_expressions",
      {},
      "Arbitrary inline parsed expressions or named Functions, one per "
      "directly initialized "
      "variable. This mode is independent of phase and region mappings");
  params.addParam<std::vector<FunctionName>>(
      "initial_region_functions",
      {},
      "Ordered inline expressions or named Functions defining regions for the "
      "compact "
      "region-mapped initialization mode");
  params.addParam<std::vector<VariableName>>(
      "initial_region_order_parameters",
      {},
      "Order parameter assigned to each initial-condition region");
  params.addParam<VariableName>("initial_background_order_parameter",
                                "Order parameter initialized outside all regions");
  params.addParam<std::vector<std::vector<Real>>>(
      "phase_initial_compositions",
      {},
      "Optional phase compositions used by generated global-composition "
      "initial conditions; "
      "phase_equilibrium_compositions are used when omitted");
  params.addRangeCheckedParam<Real>("initial_region_threshold",
                                    0.5,
                                    "initial_region_threshold >= 0 & initial_region_threshold <= 1",
                                    "Membership threshold for generated region-mapped ICs");

  params.addParam<MaterialPropertyName>("interface_mobility", "L", "Allen-Cahn mobility");
  params.addParam<MaterialPropertyName>("diffusion_mobility", "M", "Cahn-Hilliard mobility");
  params.addParam<MaterialPropertyName>("kappa", "kappa", "Gradient-energy property");
  params.addParam<MaterialPropertyName>(
      "gamma",
      "gamma",
      "Dimensionless ACGrGrMulti cross-term coefficient (gamma = 1.5 for the "
      "Moelans symmetric parameterization)");
  params.addParam<Real>(
      "allen_cahn_barrier_height", 10.0, "Barrier height passed to KKSMultiACBulkF");
  params.addParam<Real>(
      "energy_barrier_height", 1.0, "Barrier height used by the free-energy diagnostic");
  params.addParam<std::string>("barrier_order", "SIMPLE", "BarrierFunctionMaterial order");

  params.addParam<bool>("create_variables", true, "Create the KKS nonlinear variables");
  params.addParam<Real>(
      "chemical_potential_scaling", 1.0e6, "Scaling for chemical-potential variables");
  params.addParam<Real>("default_variable_scaling", 1.0, "Scaling for all other KKS variables");

  params.addParam<AuxVariableName>("boundary_aux_variable",
                                   "Optional existing auxiliary variable for BndsCalcAux");
  params.addParam<AuxVariableName>("energy_aux_variable",
                                   "Optional existing auxiliary variable for KKSMultiFreeEnergy");
  params.addParam<std::vector<AuxVariableName>>(
      "composition_aux_variables",
      {},
      "Optional existing auxiliary variables for composition-weighted "
      "switching-function sums");

  return params;
}

MultiPhaseKKSAction::MultiPhaseKKSAction(const InputParameters & parameters)
  : Action(parameters),
    _phase_names(getParam<std::vector<std::string>>("phase_names")),
    _phase_free_energies(getParam<std::vector<MaterialPropertyName>>("phase_free_energies")),
    _phase_compositions(getParam<std::vector<std::vector<VariableName>>>("phase_compositions")),
    _global_compositions(getParam<std::vector<VariableName>>("global_compositions")),
    _chemical_potentials(getParam<std::vector<VariableName>>("chemical_potentials")),
    _order_parameters(getParam<std::vector<VariableName>>("order_parameters")),
    _order_parameter_phases(getParam<std::vector<std::string>>("order_parameter_phases")),
    _switching_functions(getParam<std::vector<MaterialPropertyName>>("switching_functions")),
    _barrier_functions(getParam<std::vector<MaterialPropertyName>>("barrier_functions")),
    _phase_chemical_free_energies(
        getParam<std::vector<MaterialPropertyName>>("phase_chemical_free_energies")),
    _phase_additional_free_energies(
        getParam<std::vector<MaterialPropertyName>>("phase_additional_free_energies")),
    _phase_chemical_free_energy_expressions(
        getParam<std::vector<std::string>>("phase_chemical_free_energy_expressions")),
    _free_energy_material_properties(
        getParam<std::vector<std::string>>("free_energy_material_properties")),
    _free_energy_constant_names(getParam<std::vector<std::string>>("free_energy_constant_names")),
    _free_energy_constant_expressions(
        getParam<std::vector<std::string>>("free_energy_constant_expressions")),
    _free_energy_expression_coupled_variables(
        getParam<std::vector<VariableName>>("free_energy_expression_coupled_variables")),
    _phase_energy_offsets(getParam<std::vector<Real>>("phase_energy_offsets")),
    _phase_equilibrium_compositions(
        getParam<std::vector<std::vector<Real>>>("phase_equilibrium_compositions")),
    _phase_energy_curvatures(getParam<std::vector<std::vector<Real>>>("phase_energy_curvatures")),
    _chemical_energy_factor(getParam<Real>("chemical_energy_factor")),
    _chemical_length_scale(getParam<MaterialPropertyName>("chemical_length_scale")),
    _chemical_energy_scale(getParam<MaterialPropertyName>("chemical_energy_scale")),
    _chemical_molar_volume(getParam<MaterialPropertyName>("chemical_molar_volume")),
    _output_generated_free_energies(getParam<bool>("output_generated_free_energies")),
    _generate_expression_free_energies(!_phase_chemical_free_energy_expressions.empty()),
    _generate_quadratic_free_energies(!_phase_chemical_free_energies.empty() &&
                                      _phase_chemical_free_energy_expressions.empty()),
    _initial_condition_variables(
        getParam<std::vector<VariableName>>("initial_condition_variables")),
    _initial_condition_expressions(
        getParam<std::vector<FunctionName>>("initial_condition_expressions")),
    _initial_region_functions(getParam<std::vector<FunctionName>>("initial_region_functions")),
    _initial_region_order_parameters(
        getParam<std::vector<VariableName>>("initial_region_order_parameters")),
    _initial_background_order_parameter(
        isParamValid("initial_background_order_parameter")
            ? getParam<VariableName>("initial_background_order_parameter")
            : VariableName()),
    _initial_background_phase_index(0),
    _initial_phase_compositions(
        getParam<std::vector<std::vector<Real>>>("phase_initial_compositions")),
    _initial_region_threshold(getParam<Real>("initial_region_threshold")),
    _generate_direct_initial_conditions(!_initial_condition_expressions.empty() ||
                                        !_initial_condition_variables.empty()),
    _generate_region_initial_conditions(!_initial_region_functions.empty()),
    _generate_initial_conditions(_generate_direct_initial_conditions ||
                                 _generate_region_initial_conditions),
    _reference_phase_index(0),
    _constraint_phase_index(0),
    _interface_mobility(getParam<MaterialPropertyName>("interface_mobility")),
    _diffusion_mobility(getParam<MaterialPropertyName>("diffusion_mobility")),
    _kappa(getParam<MaterialPropertyName>("kappa")),
    _gamma(getParam<MaterialPropertyName>("gamma")),
    _allen_cahn_barrier_height(getParam<Real>("allen_cahn_barrier_height")),
    _energy_barrier_height(getParam<Real>("energy_barrier_height")),
    _barrier_order(getParam<std::string>("barrier_order")),
    _create_variables(getParam<bool>("create_variables")),
    _chemical_potential_scaling(getParam<Real>("chemical_potential_scaling")),
    _default_variable_scaling(getParam<Real>("default_variable_scaling")),
    _boundary_aux_variable(isParamValid("boundary_aux_variable")
                               ? getParam<AuxVariableName>("boundary_aux_variable")
                               : AuxVariableName()),
    _energy_aux_variable(isParamValid("energy_aux_variable")
                             ? getParam<AuxVariableName>("energy_aux_variable")
                             : AuxVariableName()),
    _composition_aux_variables(getParam<std::vector<AuxVariableName>>("composition_aux_variables"))
{
  const auto phase_count = _phase_names.size();
  const auto component_count = _global_compositions.size();
  const auto order_parameter_count = _order_parameters.size();

  if (phase_count < 2)
    paramError("phase_names", "At least two phases are required");
  if (component_count == 0)
    paramError("global_compositions", "At least one independent composition is required");
  if (order_parameter_count == 0)
    paramError("order_parameters", "At least one order parameter is required");
  if (_phase_free_energies.size() != phase_count)
    paramError("phase_free_energies", "Supply exactly one free energy per phase");
  if (_phase_compositions.size() != phase_count)
    paramError("phase_compositions", "Supply exactly one composition row per phase");
  if (_chemical_potentials.size() != component_count)
    paramError("chemical_potentials", "Supply one chemical potential per global composition");
  if (_order_parameter_phases.size() != order_parameter_count)
    paramError("order_parameter_phases", "Supply one phase assignment per order parameter");
  if (_switching_functions.size() != order_parameter_count)
    paramError("switching_functions", "Supply one switching function per order parameter");
  if (_barrier_functions.size() != order_parameter_count)
    paramError("barrier_functions", "Supply one barrier function per order parameter");
  if (!_composition_aux_variables.empty() && _composition_aux_variables.size() != component_count)
    paramError("composition_aux_variables",
               "Supply one auxiliary variable per global composition or omit "
               "the parameter");

  for (const auto & row : _phase_compositions)
    if (row.size() != component_count)
      paramError("phase_compositions",
                 "Every phase-composition row must contain one variable per "
                 "global composition");

  if (!_phase_equilibrium_compositions.empty())
  {
    if (_phase_equilibrium_compositions.size() != phase_count)
      paramError("phase_equilibrium_compositions",
                 "Supply exactly one equilibrium-composition row per phase");
    for (const auto & row : _phase_equilibrium_compositions)
      if (row.size() != component_count)
        paramError("phase_equilibrium_compositions",
                   "Every equilibrium-composition row must contain one value "
                   "per component");
  }

  const bool generate_free_energies =
      _generate_expression_free_energies || _generate_quadratic_free_energies;

  if (!generate_free_energies &&
      (!_phase_additional_free_energies.empty() || !_phase_energy_offsets.empty() ||
       !_phase_equilibrium_compositions.empty() || !_phase_energy_curvatures.empty() ||
       !_free_energy_material_properties.empty() || !_free_energy_constant_names.empty() ||
       !_free_energy_constant_expressions.empty() ||
       !_free_energy_expression_coupled_variables.empty()))
    paramError("phase_chemical_free_energies",
               "Set phase_chemical_free_energies when supplying generated "
               "free-energy data");

  if (generate_free_energies)
  {
    if (_phase_chemical_free_energies.size() != phase_count)
      paramError("phase_chemical_free_energies",
                 "Supply exactly one chemical free-energy property per phase");
    if (!_phase_additional_free_energies.empty() &&
        _phase_additional_free_energies.size() != phase_count)
      paramError("phase_additional_free_energies",
                 "Supply one additional free-energy property per phase or omit "
                 "the parameter");

    for (const auto phase : index_range(_phase_names))
      if (!_phase_additional_free_energies.empty() &&
          _phase_chemical_free_energies[phase] == _phase_free_energies[phase])
        paramError("phase_chemical_free_energies",
                   "Chemical and total free-energy names must differ when an "
                   "additional "
                   "contribution is supplied");
  }

  if (_generate_expression_free_energies)
  {
    if (_phase_chemical_free_energy_expressions.size() != phase_count)
      paramError("phase_chemical_free_energy_expressions",
                 "Supply exactly one free-energy expression per phase");
    for (const auto & expression : _phase_chemical_free_energy_expressions)
      if (expression.empty())
        paramError("phase_chemical_free_energy_expressions",
                   "Free-energy expressions must not be empty");
    if (!_phase_energy_offsets.empty() || !_phase_energy_curvatures.empty())
      paramError("phase_chemical_free_energy_expressions",
                 "Expression mode cannot be combined with phase_energy_offsets or "
                 "phase_energy_curvatures");
    if (_free_energy_constant_names.size() != _free_energy_constant_expressions.size())
      paramError("free_energy_constant_expressions",
                 "Supply one constant expression for every free-energy constant name");
    if (!_phase_equilibrium_compositions.empty())
      for (const auto phase : index_range(_phase_names))
        for (const auto component : index_range(_global_compositions))
        {
          const std::string generated_name =
              _phase_compositions[phase][component] + std::string("_eq");
          if (std::find(_free_energy_constant_names.begin(),
                        _free_energy_constant_names.end(),
                        generated_name) != _free_energy_constant_names.end())
            paramError("free_energy_constant_names",
                       generated_name +
                           " is generated from phase_equilibrium_compositions; remove the "
                           "duplicate free-energy constant");
        }
  }
  else if (!_free_energy_material_properties.empty() || !_free_energy_constant_names.empty() ||
           !_free_energy_constant_expressions.empty() ||
           !_free_energy_expression_coupled_variables.empty())
    paramError("phase_chemical_free_energy_expressions",
               "Expression-specific symbols require "
               "phase_chemical_free_energy_expressions");

  if (_generate_quadratic_free_energies)
  {
    if (_phase_energy_offsets.size() != phase_count)
      paramError("phase_energy_offsets", "Supply exactly one energy offset per phase");
    if (_phase_equilibrium_compositions.size() != phase_count)
      paramError("phase_equilibrium_compositions",
                 "Supply exactly one equilibrium-composition row per phase");
    if (_phase_energy_curvatures.size() != phase_count)
      paramError("phase_energy_curvatures", "Supply exactly one energy-curvature row per phase");

    for (const auto & row : _phase_energy_curvatures)
      if (row.size() != component_count)
        paramError("phase_energy_curvatures",
                   "Every energy-curvature row must contain one value per component");
  }

  std::unordered_map<std::string, unsigned int> phase_indices;
  for (const auto i : index_range(_phase_names))
    if (!phase_indices.emplace(_phase_names[i], i).second)
      paramError("phase_names", "Phase names must be unique");

  const auto reference = phase_indices.find(getParam<std::string>("kks_reference_phase"));
  if (reference == phase_indices.end())
    paramError("kks_reference_phase", "The reference phase is not present in phase_names");
  _reference_phase_index = reference->second;

  const auto constraint_name = getParam<std::string>("concentration_constraint_phase");
  if (constraint_name.empty())
    _constraint_phase_index = phase_count - 1;
  else
  {
    const auto constraint = phase_indices.find(constraint_name);
    if (constraint == phase_indices.end())
      paramError("concentration_constraint_phase",
                 "The concentration-constraint phase is not present in phase_names");
    _constraint_phase_index = constraint->second;
  }

  _order_parameter_phase_indices.resize(order_parameter_count);
  _free_energies_by_order_parameter.resize(order_parameter_count);
  _compositions_by_order_parameter.assign(component_count,
                                          std::vector<VariableName>(order_parameter_count));
  for (const auto i : index_range(_order_parameters))
  {
    const auto phase = phase_indices.find(_order_parameter_phases[i]);
    if (phase == phase_indices.end())
      paramError("order_parameter_phases",
                 "Every order-parameter phase must be present in phase_names");

    _order_parameter_phase_indices[i] = phase->second;
    _free_energies_by_order_parameter[i] = _phase_free_energies[phase->second];
    for (const auto component : index_range(_global_compositions))
      _compositions_by_order_parameter[component][i] =
          _phase_compositions[phase->second][component];
  }

  if (_generate_direct_initial_conditions && _generate_region_initial_conditions)
    paramError("initial_condition_expressions",
               "Direct expression initialization and compact region-mapped "
               "initialization are "
               "alternative modes; select only one");

  if (_generate_direct_initial_conditions)
  {
    if (_initial_condition_variables.size() != _initial_condition_expressions.size())
      paramError("initial_condition_expressions",
                 "Supply one initialization expression for every directly "
                 "initialized variable");
    if (!_initial_region_order_parameters.empty() || !_initial_background_order_parameter.empty() ||
        !_initial_phase_compositions.empty())
      paramError("initial_condition_expressions",
                 "Region mapping parameters cannot be used with direct "
                 "expression initialization");

    std::set<std::string> initialized_variables;
    for (const auto & variable : _initial_condition_variables)
      if (!initialized_variables.insert(variable).second)
        paramError("initial_condition_variables",
                   "Directly initialized variable names must be unique; duplicate: " + variable);
  }

  if (!_generate_initial_conditions &&
      (!_initial_region_order_parameters.empty() || !_initial_background_order_parameter.empty() ||
       !_initial_phase_compositions.empty()))
    paramError("initial_region_functions",
               "Set initial_region_functions when supplying generated "
               "initial-condition data");

  if (_generate_region_initial_conditions)
  {
    if (_initial_region_order_parameters.size() != _initial_region_functions.size())
      paramError("initial_region_order_parameters",
                 "Supply one order parameter for each initial-condition region");
    if (_initial_background_order_parameter.empty())
      paramError("initial_background_order_parameter",
                 "Select the order parameter used outside all regions");

    std::unordered_map<std::string, unsigned int> order_parameter_indices;
    for (const auto i : index_range(_order_parameters))
      order_parameter_indices.emplace(_order_parameters[i], i);

    const auto background = order_parameter_indices.find(_initial_background_order_parameter);
    if (background == order_parameter_indices.end())
      paramError("initial_background_order_parameter",
                 "The background order parameter is not present in order_parameters");
    _initial_background_phase_index = _order_parameter_phase_indices[background->second];

    std::set<std::string> initialized_order_parameters;
    _initial_region_phase_indices.reserve(_initial_region_order_parameters.size());
    for (const auto & order_parameter : _initial_region_order_parameters)
    {
      const auto op = order_parameter_indices.find(order_parameter);
      if (op == order_parameter_indices.end())
        paramError("initial_region_order_parameters",
                   "Every initial-condition order parameter must be present in "
                   "order_parameters");
      if (order_parameter == _initial_background_order_parameter)
        paramError("initial_region_order_parameters",
                   "The background order parameter cannot also define a "
                   "foreground region");
      if (!initialized_order_parameters.insert(order_parameter).second)
        paramError("initial_region_order_parameters",
                   "Use one combined region function for each initialized "
                   "order parameter");

      _initial_region_phase_indices.push_back(_order_parameter_phase_indices[op->second]);
    }

    if (_initial_phase_compositions.empty())
      _initial_phase_compositions = _phase_equilibrium_compositions;
    if (_initial_phase_compositions.size() != phase_count)
      paramError("phase_initial_compositions",
                 "Supply one initial-composition row per phase, or provide "
                 "phase_equilibrium_compositions");
    for (const auto & row : _initial_phase_compositions)
      if (row.size() != component_count)
        paramError("phase_initial_compositions",
                   "Every initial-composition row must contain one value per "
                   "component");
  }

  _free_energy_coupled_variables =
      getParam<std::vector<VariableName>>("free_energy_coupled_variables");
  if (_free_energy_coupled_variables.empty())
    for (const auto & row : _phase_compositions)
      _free_energy_coupled_variables.insert(
          _free_energy_coupled_variables.end(), row.begin(), row.end());

  std::set<std::string> variable_names;
  auto insert_unique = [&](const auto & names, const std::string & parameter)
  {
    for (const auto & name : names)
      if (!variable_names.insert(name).second)
        paramError(parameter, "KKS variable names must be unique; duplicate: " + name);
  };
  insert_unique(_global_compositions, "global_compositions");
  insert_unique(_chemical_potentials, "chemical_potentials");
  insert_unique(_order_parameters, "order_parameters");
  for (const auto & row : _phase_compositions)
    insert_unique(row, "phase_compositions");
}

void
MultiPhaseKKSAction::act()
{
  if (_current_task == "add_variable" && _create_variables)
    addVariables();
  else if (_current_task == "add_ic" && _generate_initial_conditions)
    addInitialConditions();
  else if (_current_task == "add_material")
    addMaterials();
  else if (_current_task == "add_kernel")
    addKernels();
  else if (_current_task == "add_aux_kernel")
    addAuxKernels();
}

void
MultiPhaseKKSAction::addVariables()
{
  auto add_variable = [&](const VariableName & name, Real scaling)
  {
    InputParameters params = _factory.getValidParams("MooseVariable");
    params.set<std::vector<Real>>("scaling") = {scaling};
    _problem->addVariable("MooseVariable", name, params);
  };

  for (const auto & name : _chemical_potentials)
    add_variable(name, _chemical_potential_scaling);
  for (const auto & name : _global_compositions)
    add_variable(name, _default_variable_scaling);
  for (const auto & row : _phase_compositions)
    for (const auto & name : row)
      add_variable(name, _default_variable_scaling);
  for (const auto & name : _order_parameters)
    add_variable(name, _default_variable_scaling);
}

void
MultiPhaseKKSAction::addInitialConditions()
{
  if (_generate_direct_initial_conditions)
  {
    for (const auto i : index_range(_initial_condition_variables))
    {
      InputParameters params = _factory.getValidParams("FunctionIC");
      params.set<VariableName>("variable") = _initial_condition_variables[i];
      params.set<FunctionName>("function") = _initial_condition_expressions[i];
      _problem->addInitialCondition(
          "FunctionIC", "spfi_ic_" + _initial_condition_variables[i], params);
    }
    return;
  }

  for (const auto i : index_range(_initial_region_functions))
  {
    InputParameters params = _factory.getValidParams("FunctionIC");
    params.set<VariableName>("variable") = _initial_region_order_parameters[i];
    params.set<FunctionName>("function") = _initial_region_functions[i];
    _problem->addInitialCondition(
        "FunctionIC", "spfi_ic_" + _initial_region_order_parameters[i], params);
  }

  {
    InputParameters params = _factory.getValidParams("RegionMappedIC");
    params.set<VariableName>("variable") = _initial_background_order_parameter;
    params.set<std::vector<FunctionName>>("region_functions") = _initial_region_functions;
    params.set<std::vector<Real>>("region_values") =
        std::vector<Real>(_initial_region_functions.size(), 0.0);
    params.set<Real>("background_value") = 1.0;
    params.set<Real>("threshold") = _initial_region_threshold;
    _problem->addInitialCondition(
        "RegionMappedIC", "spfi_ic_" + _initial_background_order_parameter, params);
  }

  for (const auto component : index_range(_global_compositions))
  {
    std::vector<Real> region_values;
    region_values.reserve(_initial_region_phase_indices.size());
    for (const auto phase : _initial_region_phase_indices)
      region_values.push_back(_initial_phase_compositions[phase][component]);

    InputParameters params = _factory.getValidParams("RegionMappedIC");
    params.set<VariableName>("variable") = _global_compositions[component];
    params.set<std::vector<FunctionName>>("region_functions") = _initial_region_functions;
    params.set<std::vector<Real>>("region_values") = region_values;
    params.set<Real>("background_value") =
        _initial_phase_compositions[_initial_background_phase_index][component];
    params.set<Real>("threshold") = _initial_region_threshold;
    _problem->addInitialCondition(
        "RegionMappedIC", "spfi_ic_" + _global_compositions[component], params);
  }
}

void
MultiPhaseKKSAction::addMaterials()
{
  if (_generate_expression_free_energies || _generate_quadratic_free_energies)
    addGeneratedFreeEnergies();

  for (const auto i : index_range(_order_parameters))
  {
    InputParameters params = _factory.getValidParams("SwitchingFunctionMultiPhaseMaterial");
    params.set<MaterialPropertyName>("h_name") = _switching_functions[i];
    params.set<std::vector<VariableName>>("all_etas") = _order_parameters;
    params.set<std::vector<VariableName>>("phase_etas") = {_order_parameters[i]};
    _problem->addMaterial(
        "SwitchingFunctionMultiPhaseMaterial", "spfi_" + _switching_functions[i], params);
  }

  for (const auto i : index_range(_order_parameters))
  {
    InputParameters params = _factory.getValidParams("BarrierFunctionMaterial");
    params.set<std::vector<VariableName>>("eta") = {_order_parameters[i]};
    params.set<std::string>("function_name") = _barrier_functions[i];
    params.set<MooseEnum>("g_order") = _barrier_order;
    _problem->addMaterial("BarrierFunctionMaterial", "spfi_" + _barrier_functions[i], params);
  }
}

void
MultiPhaseKKSAction::addGeneratedFreeEnergies()
{
  for (const auto phase : index_range(_phase_names))
  {
    std::string expression;
    std::vector<std::string> material_properties;
    std::vector<std::string> constant_names;
    std::vector<std::string> constant_expressions;
    std::vector<VariableName> coupled_variables = _phase_compositions[phase];

    if (_generate_expression_free_energies)
    {
      expression = _phase_chemical_free_energy_expressions[phase];
      material_properties = _free_energy_material_properties;
      constant_names = _free_energy_constant_names;
      constant_expressions = _free_energy_constant_expressions;

      if (std::find(constant_names.begin(), constant_names.end(), "chemical_energy_factor") ==
          constant_names.end())
      {
        constant_names.push_back("chemical_energy_factor");
        constant_expressions.push_back(Moose::stringify(_chemical_energy_factor));
      }

      if (!_phase_equilibrium_compositions.empty())
        for (const auto component : index_range(_global_compositions))
        {
          constant_names.push_back(_phase_compositions[phase][component] + std::string("_eq"));
          constant_expressions.push_back(
              Moose::stringify(_phase_equilibrium_compositions[phase][component]));
        }

      std::set<std::string> coupled_names(coupled_variables.begin(), coupled_variables.end());
      for (const auto & variable : _free_energy_expression_coupled_variables)
        if (coupled_names.insert(variable).second)
          coupled_variables.push_back(variable);
    }
    else
    {
      expression = "(" + _chemical_energy_scale + "/(" + _chemical_length_scale + ")^3)*((" +
                   Moose::stringify(_phase_energy_offsets[phase]) + ")";
      for (const auto component : index_range(_global_compositions))
        expression += "+(" + Moose::stringify(_phase_energy_curvatures[phase][component]) + ")*(" +
                      _phase_compositions[phase][component] + "-(" +
                      Moose::stringify(_phase_equilibrium_compositions[phase][component]) + "))^2";
      expression +=
          ")*(" + Moose::stringify(_chemical_energy_factor) + ")/" + _chemical_molar_volume;
      material_properties = {
          _chemical_length_scale, _chemical_energy_scale, _chemical_molar_volume};
    }

    {
      InputParameters params = _factory.getValidParams("DerivativeParsedMaterial");
      params.set<std::string>("property_name") = _phase_chemical_free_energies[phase];
      params.set<std::vector<VariableName>>("coupled_variables") = coupled_variables;
      params.set<std::vector<std::string>>("material_property_names") = material_properties;
      params.set<std::vector<std::string>>("constant_names") = constant_names;
      params.set<std::vector<std::string>>("constant_expressions") = constant_expressions;
      params.set<std::string>("expression") = expression;
      if (_output_generated_free_energies)
        params.set<std::vector<OutputName>>("outputs") = {"exodus"};
      _problem->addMaterial(
          "DerivativeParsedMaterial", "spfi_chemical_free_energy_" + _phase_names[phase], params);
    }

    if (_phase_additional_free_energies.empty() &&
        _phase_chemical_free_energies[phase] == _phase_free_energies[phase])
      continue;

    {
      std::vector<std::string> summands = {_phase_chemical_free_energies[phase]};
      if (!_phase_additional_free_energies.empty())
        summands.push_back(_phase_additional_free_energies[phase]);

      InputParameters params = _factory.getValidParams("DerivativeSumMaterial");
      params.set<std::string>("property_name") = _phase_free_energies[phase];
      params.set<std::vector<VariableName>>("coupled_variables") = _phase_compositions[phase];
      params.set<std::vector<std::string>>("sum_materials") = summands;
      if (_output_generated_free_energies)
        params.set<std::vector<OutputName>>("outputs") = {"exodus"};
      _problem->addMaterial(
          "DerivativeSumMaterial", "spfi_total_free_energy_" + _phase_names[phase], params);
    }
  }
}

void
MultiPhaseKKSAction::addKernels()
{
  const auto phase_count = _phase_names.size();

  for (const auto component : index_range(_global_compositions))
  {
    for (const auto phase : index_range(_phase_names))
    {
      const auto next_phase = (phase + 1) % phase_count;
      std::vector<VariableName> phase_companions;
      std::vector<VariableName> next_phase_companions;
      for (const auto other_component : index_range(_global_compositions))
        if (other_component != component)
        {
          phase_companions.push_back(_phase_compositions[phase][other_component]);
          next_phase_companions.push_back(_phase_compositions[next_phase][other_component]);
        }

      InputParameters params = _factory.getValidParams("KKSPhaseChemicalPotential");
      params.set<NonlinearVariableName>("variable") = _phase_compositions[phase][component];
      params.set<std::vector<VariableName>>("cb") = {_phase_compositions[next_phase][component]};
      params.set<MaterialPropertyName>("fa_name") = _phase_free_energies[phase];
      params.set<MaterialPropertyName>("fb_name") = _phase_free_energies[next_phase];
      params.set<std::vector<VariableName>>("args_a") = phase_companions;
      params.set<std::vector<VariableName>>("args_b") = next_phase_companions;
      _problem->addKernel("KKSPhaseChemicalPotential",
                          "spfi_kks_mu_" + _phase_names[phase] + "_" + _phase_names[next_phase] +
                              "_" + _global_compositions[component],
                          params);
    }

    {
      InputParameters params = _factory.getValidParams("KKSMultiPhaseConcentration");
      params.set<NonlinearVariableName>("variable") =
          _phase_compositions[_constraint_phase_index][component];
      params.set<std::vector<VariableName>>("cj") = _compositions_by_order_parameter[component];
      params.set<std::vector<MaterialPropertyName>>("hj_names") = _switching_functions;
      params.set<std::vector<VariableName>>("etas") = _order_parameters;
      params.set<std::vector<VariableName>>("c") = {_global_compositions[component]};
      _problem->addKernel("KKSMultiPhaseConcentration",
                          "spfi_phase_concentration_" + _global_compositions[component],
                          params);
    }

    {
      std::vector<VariableName> companion_compositions;
      for (const auto other_component : index_range(_global_compositions))
        if (other_component != component)
          companion_compositions.push_back(
              _phase_compositions[_reference_phase_index][other_component]);

      InputParameters params = _factory.getValidParams("KKSSplitCHCRes");
      params.set<NonlinearVariableName>("variable") = _global_compositions[component];
      params.set<std::vector<VariableName>>("ca") = {
          _phase_compositions[_reference_phase_index][component]};
      params.set<MaterialPropertyName>("fa_name") = _phase_free_energies[_reference_phase_index];
      params.set<std::vector<VariableName>>("w") = {_chemical_potentials[component]};
      params.set<std::vector<VariableName>>("args_a") = companion_compositions;
      _problem->addKernel(
          "KKSSplitCHCRes", "spfi_ch_bulk_" + _global_compositions[component], params);
    }

    {
      InputParameters params = _factory.getValidParams("CoupledTimeDerivative");
      params.set<NonlinearVariableName>("variable") = _chemical_potentials[component];
      params.set<std::vector<VariableName>>("v") = {_global_compositions[component]};
      _problem->addKernel(
          "CoupledTimeDerivative", "spfi_dcdt_" + _global_compositions[component], params);
    }

    {
      InputParameters params = _factory.getValidParams("SplitCHWRes");
      params.set<NonlinearVariableName>("variable") = _chemical_potentials[component];
      params.set<MaterialPropertyName>("mob_name") = _diffusion_mobility;
      params.set<std::vector<VariableName>>("coupled_variables") = _order_parameters;
      _problem->addKernel("SplitCHWRes", "spfi_ch_flux_" + _global_compositions[component], params);
    }
  }

  for (const auto i : index_range(_order_parameters))
  {
    const auto others = otherOrderParameters(i);

    {
      InputParameters params = _factory.getValidParams("TimeDerivative");
      params.set<NonlinearVariableName>("variable") = _order_parameters[i];
      _problem->addKernel("TimeDerivative", "spfi_dt_" + _order_parameters[i], params);
    }

    {
      auto coupled = _free_energy_coupled_variables;
      coupled.insert(coupled.end(), others.begin(), others.end());
      InputParameters params = _factory.getValidParams("KKSMultiACBulkF");
      params.set<NonlinearVariableName>("variable") = _order_parameters[i];
      params.set<std::vector<MaterialPropertyName>>("Fj_names") = _free_energies_by_order_parameter;
      params.set<std::vector<MaterialPropertyName>>("hj_names") = _switching_functions;
      params.set<MaterialPropertyName>("gi_name") = _barrier_functions[i];
      params.set<std::vector<VariableName>>("eta_i") = {_order_parameters[i]};
      params.set<Real>("wi") = _allen_cahn_barrier_height;
      params.set<std::vector<VariableName>>("coupled_variables") = coupled;
      params.set<MaterialPropertyName>("mob_name") = _interface_mobility;
      _problem->addKernel("KKSMultiACBulkF", "spfi_ac_bulk_f_" + _order_parameters[i], params);
    }

    for (const auto component : index_range(_global_compositions))
    {
      auto coupled = others;
      for (const auto other_component : index_range(_global_compositions))
        if (other_component != component)
          for (const auto phase : index_range(_phase_names))
            coupled.push_back(_phase_compositions[phase][other_component]);

      InputParameters params = _factory.getValidParams("KKSMultiACBulkC");
      params.set<NonlinearVariableName>("variable") = _order_parameters[i];
      params.set<std::vector<MaterialPropertyName>>("Fj_names") = _free_energies_by_order_parameter;
      params.set<std::vector<MaterialPropertyName>>("hj_names") = _switching_functions;
      params.set<std::vector<VariableName>>("cj_names") =
          _compositions_by_order_parameter[component];
      params.set<std::vector<VariableName>>("eta_i") = {_order_parameters[i]};
      params.set<std::vector<VariableName>>("coupled_variables") = coupled;
      params.set<MaterialPropertyName>("mob_name") = _interface_mobility;
      _problem->addKernel("KKSMultiACBulkC",
                          "spfi_ac_bulk_c_" + _global_compositions[component] + "_" +
                              _order_parameters[i],
                          params);
    }

    {
      InputParameters params = _factory.getValidParams("ACInterface");
      params.set<NonlinearVariableName>("variable") = _order_parameters[i];
      params.set<MaterialPropertyName>("kappa_name") = _kappa;
      params.set<MaterialPropertyName>("mob_name") = _interface_mobility;
      _problem->addKernel("ACInterface", "spfi_ac_interface_" + _order_parameters[i], params);
    }

    {
      // ACGrGrMulti obtains the homogeneous barrier energy density through its
      // required material property named "mu". In the notation of Moelans et
      // al. (Phys. Rev. B 78, 024113, 2008), this is m, not the physical grain-
      // boundary mobility mu_gb. For gamma = 1.5, m = 6*sigma_gb/ell_gb.
      InputParameters params = _factory.getValidParams("ACGrGrMulti");
      params.set<NonlinearVariableName>("variable") = _order_parameters[i];
      params.set<std::vector<VariableName>>("v") = others;
      params.set<std::vector<MaterialPropertyName>>("gamma_names") =
          repeatedProperty(_gamma, others.size());
      params.set<MaterialPropertyName>("mob_name") = _interface_mobility;
      params.set<std::vector<VariableName>>("coupled_variables") = others;
      _problem->addKernel("ACGrGrMulti", "spfi_ac_cross_" + _order_parameters[i], params);
    }
  }
}

void
MultiPhaseKKSAction::addAuxKernels()
{
  if (!_boundary_aux_variable.empty())
  {
    InputParameters params = _factory.getValidParams("BndsCalcAux");
    params.set<AuxVariableName>("variable") = _boundary_aux_variable;
    params.set<std::vector<VariableName>>("v") = _order_parameters;
    _problem->addAuxKernel("BndsCalcAux", "spfi_phase_boundaries", params);
  }

  if (!_energy_aux_variable.empty())
  {
    InputParameters params = _factory.getValidParams("KKSMultiFreeEnergy");
    params.set<AuxVariableName>("variable") = _energy_aux_variable;
    params.set<std::vector<MaterialPropertyName>>("Fj_names") = _free_energies_by_order_parameter;
    params.set<std::vector<MaterialPropertyName>>("hj_names") = _switching_functions;
    params.set<std::vector<MaterialPropertyName>>("gj_names") = _barrier_functions;
    params.set<Real>("w") = _energy_barrier_height;
    params.set<std::vector<VariableName>>("interfacial_vars") = _order_parameters;
    params.set<std::vector<MaterialPropertyName>>("kappa_names") =
        repeatedProperty(_kappa, _order_parameters.size());
    _problem->addAuxKernel("KKSMultiFreeEnergy", "spfi_total_free_energy", params);
  }

  for (const auto component : index_range(_composition_aux_variables))
  {
    InputParameters params = _factory.getValidParams("SwitchingFunctionSquaredSumAux");
    params.set<AuxVariableName>("variable") = _composition_aux_variables[component];
    params.set<std::vector<VariableName>>("composition") = {_global_compositions[component]};
    params.set<std::vector<MaterialPropertyName>>("switching_functions") = _switching_functions;
    _problem->addAuxKernel("SwitchingFunctionSquaredSumAux",
                           "spfi_weighted_" + _global_compositions[component],
                           params);
  }
}

std::vector<VariableName>
MultiPhaseKKSAction::otherOrderParameters(unsigned int index) const
{
  std::vector<VariableName> others;
  others.reserve(_order_parameters.size() - 1);
  for (const auto i : index_range(_order_parameters))
    if (i != index)
      others.push_back(_order_parameters[i]);
  return others;
}

std::vector<MaterialPropertyName>
MultiPhaseKKSAction::repeatedProperty(const MaterialPropertyName & property,
                                      unsigned int count) const
{
  return std::vector<MaterialPropertyName>(count, property);
}
