/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef KINETICS_H
#define KINETICS_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

class Kinetics: public DerivativeMaterialInterface<Kernel>
{
public:
  Kinetics(const InputParameters & parameters);
  // Declaration of validParams() without template specialization
  static InputParameters validParams();

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

private:
  // Coupled variables
  unsigned int _cp_var;
  const VariableValue & _cp;
  unsigned int _cv_var;
  const VariableValue & _cv;

  // Material properties
  const MaterialProperty<Real> & _F;
  const MaterialProperty<Real> & _dFe;
  const MaterialProperty<Real> & _dFv;
  const MaterialProperty<Real> & _dF;
};

#endif // KINETICS_H

