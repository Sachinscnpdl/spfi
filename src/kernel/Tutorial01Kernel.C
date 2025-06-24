

#include "Tutorial01Kernel.h"
//\\ This kernel acts like a source kernel
registerMooseObject("spfiApp", Tutorial01Kernel);

InputParameters 
Tutorial01Kernel::validParams()
{
  auto params = ADKernel::validParams();
  params.addClassDescription("This kernel is linear elastic .....");
  params.addRequiredCoupledVar("displacements", "The displacement variables for the problem.");
  params.addParam<MaterialPropertyName>("shear_modulus", "Shear modolus of the material");
  params.addParam<MaterialPropertyName>("lambda", "Lame's first parameter of the material");
  params.addRequiredParam<unsigned int>("component", "The component of the residuals");


  return params;
}

Tutorial01Kernel::Tutorial01Kernel(const InputParameters & params) :
    ADKernel(params),
_grad_disp(adCoupledGradients("displacements")),
_mu(getADMaterialProperty<Real>("shear_modulus")),
_lambda(getADMaterialProperty<Real>("lambda")),
_component(getParam<unsigned int>("component"))
{
  _grad_disp.resize(3, &_ad_grad_zero);
}

ADReal 
Tutorial01Kernel::computeQpResidual()
{
  using R2 = ADRankTwoTensor;

  // Compute strain from displacement gradients
  auto G = R2::initializeFromRows((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  auto e = 0.5*(G.transpose()+G);

  // Compute stress from strain
  auto s = _lambda[_qp] * e.tr() *R2::Identity() + 2* _mu[_qp] *e;

  // Compute residual
  return _grad_test[_i][_qp] * s.row(_component);
}
