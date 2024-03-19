//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef FIVEPHASESSUMCDOTHSQUARE_H
#define FIVEPHASESSUMCDOTHSQUARE_H

#include "AuxKernel.h"

// Forward Declarations
class FivePhasesSumCdothsquare;


/**
 * Auxiliary kernel responsible for computing the Darcy velocity given
 * several fluid properties and the pressure gradient.
 */
class FivePhasesSumCdothsquare : public AuxKernel
{
public:
  static InputParameters validParams();
  FivePhasesSumCdothsquare(const InputParameters & parameters);

protected:
  /**
   * AuxKernels MUST override computeValue.  computeValue() is called on
   * every quadrature point.  For Nodal Auxiliary variables those quadrature
   * points coincide with the nodes.
   */
  virtual Real computeValue() override;

  /// Value of the coupled variable
  const VariableValue & _var1;

  /// The gradient of a coupled variable
  // const VariableGradient & _var1_gradient;

  const MaterialProperty<Real> & _prop_h1;
  const MaterialProperty<Real> & _prop_h2;
  const MaterialProperty<Real> & _prop_h3;
  const MaterialProperty<Real> & _prop_h4;
  const MaterialProperty<Real> & _prop_h5;
};

#endif // FIVEPHASESSUMCDOTHSQUARE_H
