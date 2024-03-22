/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef CONDUCTION_H
#define CONDUCTION_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

class Conduction: public DerivativeMaterialInterface<Kernel>
{
public:
  Conduction(const InputParameters & parameters);

  // Declaration of validParams() without template specialization
  static InputParameters validParams();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  unsigned int _cp_var;
  const VariableValue & _cp;
  unsigned int _cv_var;
  const VariableValue & _cv;
  const VariableGradient & _grad_cp;
  const MaterialProperty<Real> & _Q;
  const MaterialProperty<Real> & _QM;
  const MaterialProperty<Real> & _dQ;
  const MaterialProperty<Real> & _dQv;
  const MaterialProperty<Real> & _dQMv;
  const MaterialProperty<Real> & _dQM;
};

#endif // CONDUCTION_H

