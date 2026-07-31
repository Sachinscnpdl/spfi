//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#include "SwitchingFunctionSquaredSumAux.h"

registerMooseObject("spfiApp", SwitchingFunctionSquaredSumAux);

InputParameters
SwitchingFunctionSquaredSumAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Computes a composition multiplied by the sum of "
                             "squared switching functions.");
  params.addRequiredCoupledVar("composition", "Composition variable to weight the sum");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "switching_functions", "Switching-function material properties to square and sum");
  return params;
}

SwitchingFunctionSquaredSumAux::SwitchingFunctionSquaredSumAux(const InputParameters & parameters)
  : AuxKernel(parameters), _composition(coupledValue("composition"))
{
  const auto & names = getParam<std::vector<MaterialPropertyName>>("switching_functions");
  if (names.empty())
    paramError("switching_functions", "At least one switching function is required");

  _switching_functions.reserve(names.size());
  for (const auto & name : names)
    _switching_functions.push_back(&getMaterialProperty<Real>(name));
}

Real
SwitchingFunctionSquaredSumAux::computeValue()
{
  Real sum = 0.0;
  for (const auto * h : _switching_functions)
    sum += (*h)[_qp] * (*h)[_qp];

  return _composition[_qp] * sum;
}
