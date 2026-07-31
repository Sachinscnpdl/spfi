//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#pragma once

#include "Action.h"

/**
 * Creates phase-field diagnostics from order-parameter and phase mappings
 * without depending on a specific free-energy or mechanics formulation.
 */
class PhaseFieldDiagnosticsAction : public Action
{
public:
  static InputParameters validParams();

  PhaseFieldDiagnosticsAction(const InputParameters & parameters);

  virtual void act() override;

protected:
  const std::vector<std::string> _phase_names;
  std::vector<std::string> _phase_labels;
  const std::vector<VariableName> _order_parameters;
  const std::vector<std::string> _order_parameter_phases;
  const std::vector<MaterialPropertyName> _switching_functions;
  const std::string _area_prefix;
  const ExecFlagEnum _execute_on;
  const std::vector<SubdomainName> _blocks;

  std::vector<PostprocessorName> _area_postprocessor_names;
};
