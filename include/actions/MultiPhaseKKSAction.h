//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#pragma once

#include "Action.h"

/**
 * Builds the repetitive variables, materials, kernels, and diagnostics for a
 * multi-phase KKS model without hard-coding the number of phases, components,
 * or order parameters.
 */
class MultiPhaseKKSAction : public Action
{
public:
  static InputParameters validParams();

  MultiPhaseKKSAction(const InputParameters & parameters);

  virtual void act() override;

protected:
  void addVariables();
  void addInitialConditions();
  void addMaterials();
  void addKernels();
  void addAuxKernels();

  void addGeneratedFreeEnergies();

  std::vector<VariableName> otherOrderParameters(unsigned int index) const;
  std::vector<MaterialPropertyName> repeatedProperty(const MaterialPropertyName & property,
                                                     unsigned int count) const;

  const std::vector<std::string> _phase_names;
  const std::vector<MaterialPropertyName> _phase_free_energies;
  const std::vector<std::vector<VariableName>> _phase_compositions;
  const std::vector<VariableName> _global_compositions;
  const std::vector<VariableName> _chemical_potentials;
  const std::vector<VariableName> _order_parameters;
  const std::vector<std::string> _order_parameter_phases;
  const std::vector<MaterialPropertyName> _switching_functions;
  const std::vector<MaterialPropertyName> _barrier_functions;

  const std::vector<MaterialPropertyName> _phase_chemical_free_energies;
  const std::vector<MaterialPropertyName> _phase_additional_free_energies;
  const std::vector<std::string> _phase_chemical_free_energy_expressions;
  const std::vector<std::string> _free_energy_material_properties;
  const std::vector<std::string> _free_energy_constant_names;
  const std::vector<std::string> _free_energy_constant_expressions;
  const std::vector<VariableName> _free_energy_expression_coupled_variables;
  const std::vector<Real> _phase_energy_offsets;
  const std::vector<std::vector<Real>> _phase_equilibrium_compositions;
  const std::vector<std::vector<Real>> _phase_energy_curvatures;
  const Real _chemical_energy_factor;
  const MaterialPropertyName _chemical_length_scale;
  const MaterialPropertyName _chemical_energy_scale;
  const MaterialPropertyName _chemical_molar_volume;
  const bool _output_generated_free_energies;
  const bool _generate_expression_free_energies;
  const bool _generate_quadratic_free_energies;

  const std::vector<VariableName> _initial_condition_variables;
  const std::vector<FunctionName> _initial_condition_expressions;
  const std::vector<FunctionName> _initial_region_functions;
  const std::vector<VariableName> _initial_region_order_parameters;
  const VariableName _initial_background_order_parameter;
  std::vector<unsigned int> _initial_region_phase_indices;
  unsigned int _initial_background_phase_index;
  std::vector<std::vector<Real>> _initial_phase_compositions;
  const Real _initial_region_threshold;
  const bool _generate_direct_initial_conditions;
  const bool _generate_region_initial_conditions;
  const bool _generate_initial_conditions;

  std::vector<unsigned int> _order_parameter_phase_indices;
  std::vector<MaterialPropertyName> _free_energies_by_order_parameter;
  std::vector<std::vector<VariableName>> _compositions_by_order_parameter;
  std::vector<VariableName> _free_energy_coupled_variables;

  unsigned int _reference_phase_index;
  unsigned int _constraint_phase_index;

  const MaterialPropertyName _interface_mobility;
  const MaterialPropertyName _diffusion_mobility;
  const MaterialPropertyName _kappa;
  const MaterialPropertyName _gamma;
  const Real _allen_cahn_barrier_height;
  const Real _energy_barrier_height;
  const std::string _barrier_order;

  const bool _create_variables;
  const Real _chemical_potential_scaling;
  const Real _default_variable_scaling;

  const AuxVariableName _boundary_aux_variable;
  const AuxVariableName _energy_aux_variable;
  const std::vector<AuxVariableName> _composition_aux_variables;
};
