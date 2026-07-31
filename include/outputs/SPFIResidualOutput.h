#pragma once

#include "PetscOutput.h"

/**
 * Prints SPFI linear and nonlinear solver histories with trend-aware colors.
 */
class SPFIResidualOutput : public PetscOutput
{
public:
  static InputParameters validParams();

  SPFIResidualOutput(const InputParameters & parameters);

protected:
  void output() override;

private:
  static const char * ansi(const char * code);
  static std::string coloredNorm(Real old_norm, Real norm, unsigned int precision = 6);

  Real _old_linear_norm;
  Real _old_nonlinear_norm;
};
