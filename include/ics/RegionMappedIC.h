//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details

#pragma once

#include "InitialCondition.h"

class Function;

/**
 * Assigns a value according to the first region function above a threshold,
 * falling back to a background value outside every region.
 */
class RegionMappedIC : public InitialCondition
{
public:
  static InputParameters validParams();

  RegionMappedIC(const InputParameters & parameters);

protected:
  virtual Real value(const Point & p) override;

  std::vector<const Function *> _region_functions;
  const std::vector<Real> _region_values;
  const Real _background_value;
  const Real _threshold;
};
