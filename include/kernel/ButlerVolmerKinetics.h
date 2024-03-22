//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BUTLERVOLMERKINETICS_H
#define BUTLERVOLMERKINETICS_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

class ButlerVolmerKinetics: public DerivativeMaterialInterface<Kernel>
{
public:
  ButlerVolmerKinetics(const InputParameters & parameters);

  // Declaration of validParams() without template specialization
  static InputParameters validParams();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  unsigned int _c_pot_var;
  const VariableValue & _c_pot;
  unsigned int _c_eta_var;
  const VariableValue & _c_eta;
  unsigned int _c_comp_var;
  const VariableValue & _c_comp;
  const MaterialProperty<Real> & _F;
  const MaterialProperty<Real> & _dF;
  const MaterialProperty<Real> & _dF_c_eta;
  const MaterialProperty<Real> & _dF_c_pot;
  const MaterialProperty<Real> & _dF_c_comp;
};

#endif // BUTLERVOLMERKINETICS_H

