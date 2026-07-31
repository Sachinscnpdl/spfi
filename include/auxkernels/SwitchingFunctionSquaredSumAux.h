//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#pragma once

#include "AuxKernel.h"

/**
 * Computes c * sum_i(h_i^2) for an arbitrary number of switching functions.
 */
class SwitchingFunctionSquaredSumAux : public AuxKernel
{
public:
  static InputParameters validParams();

  SwitchingFunctionSquaredSumAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  const VariableValue & _composition;
  std::vector<const MaterialProperty<Real> *> _switching_functions;
};
