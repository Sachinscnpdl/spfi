/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "ButlerVolmerKinetics.h"

registerMooseObject("spfiApp", ButlerVolmerKinetics);

// Definition of validParams() as a member function
#include "ButlerVolmerKinetics.h"

// Definition of validParams() without template specialization
InputParameters ButlerVolmerKinetics::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Add in ButlerVolmerKinetics");
  params.addRequiredCoupledVar("c_pot", "potential as coupled variable");
  params.addRequiredCoupledVar("c_eta", "coupled eta as coupled variable 2");
  params.addRequiredCoupledVar("c_comp", "coupled comp as coupled variable 3");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  return params;
}

ButlerVolmerKinetics::ButlerVolmerKinetics(const InputParameters & parameters)
: DerivativeMaterialInterface<Kernel>(parameters),
  _c_pot_var(coupled("c_pot")),
  _c_pot(coupledValue("c_pot")),
  _c_eta_var(coupled("c_eta")),
  _c_eta(coupledValue("c_eta")),
  _c_comp_var(coupled("c_comp")),
  _c_comp(coupledValue("c_comp")),
  _F(getMaterialProperty<Real>("f_name")),
  _dF(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
  _dF_c_eta(getMaterialPropertyDerivative<Real>("f_name", getVar("c_eta", 0)->name())),
  _dF_c_pot(getMaterialPropertyDerivative<Real>("f_name", getVar("c_pot", 0)->name())),
  _dF_c_comp(getMaterialPropertyDerivative<Real>("f_name", getVar("c_comp", 0)->name()))
{
}

Real ButlerVolmerKinetics::computeQpResidual()
{
  return _F[_qp] * _test[_i][_qp];
}

Real ButlerVolmerKinetics::computeQpJacobian()
{
  return _dF[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real ButlerVolmerKinetics::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_pot_var)
    return _dF_c_pot[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _c_eta_var)
    return _dF_c_eta[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _c_comp_var)
    return _dF_c_comp[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else
    return 0;
}

