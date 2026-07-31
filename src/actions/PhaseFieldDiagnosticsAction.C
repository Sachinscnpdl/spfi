//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#include "PhaseFieldDiagnosticsAction.h"

#include "FEProblem.h"
#include "Factory.h"
#include "MooseUtils.h"

#include <set>
#include <unordered_map>

registerMooseAction("spfiApp", PhaseFieldDiagnosticsAction, "add_postprocessor");

InputParameters
PhaseFieldDiagnosticsAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription(
      "Creates one switching-function area integral for every mapped order parameter.");

  params.addRequiredParam<std::vector<std::string>>("phase_names", "Unique phase names");
  params.addParam<std::vector<std::string>>(
      "phase_labels", {}, "Optional output label for each phase; phase_names are used by default");
  params.addRequiredParam<std::vector<VariableName>>("order_parameters",
                                                     "Order parameters to report");
  params.addRequiredParam<std::vector<std::string>>("order_parameter_phases",
                                                    "Phase assigned to each order parameter");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "switching_functions", "Switching-function property for each order parameter");

  params.addParam<std::string>("area_prefix", "area", "Prefix for generated postprocessor names");
  ExecFlagEnum execute_on = MooseUtils::getDefaultExecFlagEnum();
  execute_on = {EXEC_INITIAL, EXEC_TIMESTEP_END};
  params.addParam<ExecFlagEnum>(
      "execute_on", execute_on, "Execution schedule for generated area postprocessors");
  params.addParam<std::vector<SubdomainName>>(
      "block", {}, "Optional blocks included in all generated area integrals");

  return params;
}

PhaseFieldDiagnosticsAction::PhaseFieldDiagnosticsAction(const InputParameters & parameters)
  : Action(parameters),
    _phase_names(getParam<std::vector<std::string>>("phase_names")),
    _phase_labels(getParam<std::vector<std::string>>("phase_labels")),
    _order_parameters(getParam<std::vector<VariableName>>("order_parameters")),
    _order_parameter_phases(getParam<std::vector<std::string>>("order_parameter_phases")),
    _switching_functions(getParam<std::vector<MaterialPropertyName>>("switching_functions")),
    _area_prefix(getParam<std::string>("area_prefix")),
    _execute_on(getParam<ExecFlagEnum>("execute_on")),
    _blocks(getParam<std::vector<SubdomainName>>("block"))
{
  const auto phase_count = _phase_names.size();
  const auto order_parameter_count = _order_parameters.size();
  if (phase_count == 0)
    paramError("phase_names", "At least one phase is required");
  if (order_parameter_count == 0)
    paramError("order_parameters", "At least one order parameter is required");
  if (_order_parameter_phases.size() != order_parameter_count)
    paramError("order_parameter_phases", "Supply one phase per order parameter");
  if (_switching_functions.size() != order_parameter_count)
    paramError("switching_functions", "Supply one switching function per order parameter");
  if (!_phase_labels.empty() && _phase_labels.size() != phase_count)
    paramError("phase_labels", "Supply one output label per phase or omit the parameter");
  if (_area_prefix.empty())
    paramError("area_prefix", "The generated postprocessor prefix cannot be empty");

  if (_phase_labels.empty())
    _phase_labels = _phase_names;

  std::unordered_map<std::string, unsigned int> phase_indices;
  for (const auto phase : index_range(_phase_names))
    if (!phase_indices.emplace(_phase_names[phase], phase).second)
      paramError("phase_names", "Phase names must be unique");

  std::set<std::string> postprocessor_names;
  _area_postprocessor_names.reserve(order_parameter_count);
  for (const auto i : index_range(_order_parameters))
  {
    const auto phase = phase_indices.find(_order_parameter_phases[i]);
    if (phase == phase_indices.end())
      paramError("order_parameter_phases",
                 "Every order-parameter phase must be present in phase_names");

    const PostprocessorName name =
        _area_prefix + "_" + _phase_labels[phase->second] + "_" + _order_parameters[i];
    if (!postprocessor_names.insert(name).second)
      paramError("order_parameters", "Generated area postprocessor names must be unique");
    _area_postprocessor_names.push_back(name);
  }
}

void
PhaseFieldDiagnosticsAction::act()
{
  for (const auto i : index_range(_order_parameters))
  {
    InputParameters params = _factory.getValidParams("ElementIntegralMaterialProperty");
    params.set<MaterialPropertyName>("mat_prop") = _switching_functions[i];
    params.set<ExecFlagEnum>("execute_on") = _execute_on;
    if (!_blocks.empty())
      params.set<std::vector<SubdomainName>>("block") = _blocks;
    _problem->addPostprocessor(
        "ElementIntegralMaterialProperty", _area_postprocessor_names[i], params);
  }
}
