//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ComputeLagrangianStressPK1.h"
#include "SingleVariableReturnMappingSolution.h"

class ComputeNeoHookeanJ2Stress : public ComputeLagrangianStressPK1,
                                  public SingleVariableReturnMappingSolution
{
public:
  static InputParameters validParams();
  ComputeNeoHookeanJ2Stress(const InputParameters & parameters);

protected:
  virtual void initialSetup() override;
  virtual void initQpStatefulProperties() override;
  virtual void computeQpPK1Stress() override;

  virtual Real computeReferenceResidual(const Real & effective_trial_stress,
                                        const Real & scalar) override;
  virtual Real computeResidual(const Real & effective_trial_stress, const Real & scalar) override;
  virtual Real computeDerivative(const Real & effective_trial_stress, const Real & scalar) override;
  virtual void preStep(const Real & scalar, const Real & R, const Real & J) override;

private:
  const MaterialProperty<Real> & _lambda;
  const MaterialProperty<Real> & _mu;
  const MaterialProperty<RankTwoTensor> & _F_old;

  MaterialProperty<Real> & _ep;
  const MaterialProperty<Real> & _ep_old;
  MaterialProperty<RankTwoTensor> & _be;
  const MaterialProperty<RankTwoTensor> & _be_old;
  MaterialProperty<RankTwoTensor> & _Np;

  Material * _flow_stress_material;
  const MaterialProperty<Real> & _H;
  const MaterialProperty<Real> & _dH;
  const MaterialProperty<Real> & _d2H;

  RankFourTensor _d_be_d_F;
  RankFourTensor _d_n_d_be;
  RankFourTensor _d_deltaep_d_betr;
  RankFourTensor _d_R_d_betr;
  RankFourTensor _d_J_d_betr;
};
