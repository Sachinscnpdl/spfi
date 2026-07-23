#pragma once

#include "ADKernel.h"
#include "RankTwoTensor.h"


class Tutorial01Kernel : public ADKernel
{
public:
  static InputParameters validParams();
  Tutorial01Kernel(const InputParameters & parameters);

protected:
virtual ADReal computeQpResidual() override;

std::vector<const ADVariableGradient *> _grad_disp;
const ADMaterialProperty<Real> & _mu;
const ADMaterialProperty<Real> & _lambda;
const unsigned int _component;
};


