//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#include "RegionMappedIC.h"

#include "Function.h"

registerMooseObject("spfiApp", RegionMappedIC);

InputParameters
RegionMappedIC::validParams()
{
  InputParameters params = InitialCondition::validParams();
  params.addRequiredParam<std::vector<FunctionName>>(
      "region_functions", "Functions that identify regions in first-match order");
  params.addRequiredParam<std::vector<Real>>("region_values",
                                             "Value assigned inside each corresponding region");
  params.addParam<Real>("background_value", 0.0, "Value outside all regions");
  params.addRangeCheckedParam<Real>(
      "threshold", 0.5, "threshold >= 0 & threshold <= 1", "Region membership threshold");
  params.addClassDescription(
      "Maps ordered region functions to initial values with a background fallback.");
  return params;
}

RegionMappedIC::RegionMappedIC(const InputParameters & parameters)
  : InitialCondition(parameters),
    _region_values(getParam<std::vector<Real>>("region_values")),
    _background_value(getParam<Real>("background_value")),
    _threshold(getParam<Real>("threshold"))
{
  const auto & function_names = getParam<std::vector<FunctionName>>("region_functions");
  if (function_names.size() != _region_values.size())
    paramError("region_values", "Supply one value for each region function");

  _region_functions.reserve(function_names.size());
  for (const auto & function_name : function_names)
    _region_functions.push_back(&getFunctionByName(function_name));
}

Real
RegionMappedIC::value(const Point & p)
{
  for (const auto i : index_range(_region_functions))
    if (_region_functions[i]->value(_t, p) > _threshold)
      return _region_values[i];

  return _background_value;
}
