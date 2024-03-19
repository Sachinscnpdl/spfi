//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ElevenPhasesSumCdothsquare.h"

registerMooseObject("spfiApp", ElevenPhasesSumCdothsquare);

InputParameters
ElevenPhasesSumCdothsquare::validParams()
{
  InputParameters params =  AuxKernel::validParams();

  // Add a "coupling paramater" to get a variable from the input file.
  params.addRequiredCoupledVar("var1", "order parameter as coupled variable."); //var1=global composition

  params.addRequiredParam<MaterialPropertyName>("h1_name","Switching function of eta1");
  params.addRequiredParam<MaterialPropertyName>("h2_name","Switching function of eta2");
  params.addRequiredParam<MaterialPropertyName>("h3_name","Switching function of eta3");
  params.addRequiredParam<MaterialPropertyName>("h4_name","Switching function of eta4");
  params.addRequiredParam<MaterialPropertyName>("h5_name","Switching function of eta5");
  params.addRequiredParam<MaterialPropertyName>("h6_name","Switching function of eta6");
  params.addRequiredParam<MaterialPropertyName>("h7_name","Switching function of eta7");
  params.addRequiredParam<MaterialPropertyName>("h8_name","Switching function of eta8");
  params.addRequiredParam<MaterialPropertyName>("h9_name","Switching function of eta9");
  params.addRequiredParam<MaterialPropertyName>("h10_name","Switching function of eta10");
  params.addRequiredParam<MaterialPropertyName>("h11_name","Switching function of eta11");

  return params;
}

ElevenPhasesSumCdothsquare::ElevenPhasesSumCdothsquare(const InputParameters & parameters)
  : AuxKernel(parameters),

     // We can couple in a value from one of our kernels with a call to coupledValueAux
    _var1(coupledValue("var1")),

    // Get the gradient of the variable
    //_var1_gradient(coupledGradient("var1")),

    _prop_h1(getMaterialProperty<Real>("h1_name")),
    _prop_h2(getMaterialProperty<Real>("h2_name")),
    _prop_h3(getMaterialProperty<Real>("h3_name")),
    _prop_h4(getMaterialProperty<Real>("h4_name")),
    _prop_h5(getMaterialProperty<Real>("h5_name")),
    _prop_h6(getMaterialProperty<Real>("h6_name")),
    _prop_h7(getMaterialProperty<Real>("h7_name")),
    _prop_h8(getMaterialProperty<Real>("h8_name")),
    _prop_h9(getMaterialProperty<Real>("h9_name")),
    _prop_h10(getMaterialProperty<Real>("h10_name")),
    _prop_h11(getMaterialProperty<Real>("h11_name"))
{
}

Real
ElevenPhasesSumCdothsquare::computeValue()
{
  return _var1[_qp]*(_prop_h1[_qp]*_prop_h1[_qp]+_prop_h2[_qp]*_prop_h2[_qp]+_prop_h3[_qp]*_prop_h3[_qp]+_prop_h4[_qp]*_prop_h4[_qp]+_prop_h5[_qp]*_prop_h5[_qp]+_prop_h6[_qp]*_prop_h6[_qp]+_prop_h7[_qp]*_prop_h7[_qp]+_prop_h8[_qp]*_prop_h8[_qp]+_prop_h9[_qp]*_prop_h9[_qp]+_prop_h10[_qp]*_prop_h10[_qp]+_prop_h11[_qp]*_prop_h11[_qp]) ;
}
