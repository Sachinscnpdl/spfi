#include "SPFIResidualOutput.h"

#include "Moose.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

registerMooseObject("spfiApp", SPFIResidualOutput);

namespace
{
constexpr const char * RESIDUAL_RESET = "\033[0m";
constexpr const char * RESIDUAL_BAD_HIGHLIGHT = "\033[1;97;41m";
constexpr const char * RESIDUAL_GOOD_HIGHLIGHT = "\033[1;30;102m";
constexpr const char * RESIDUAL_SLOW_HIGHLIGHT = "\033[1;97;105m";
constexpr const char * RESIDUAL_BRIGHT_BLUE = "\033[94m";
constexpr const char * RESIDUAL_BRIGHT_MAGENTA = "\033[95m";
}

InputParameters
SPFIResidualOutput::validParams()
{
  InputParameters params = PetscOutput::validParams();
  params.addClassDescription(
      "Prints SPFI linear and nonlinear residual histories with trend-aware colors.");
  params.set<bool>("output_linear") = true;
  params.set<bool>("output_nonlinear") = true;
  return params;
}

SPFIResidualOutput::SPFIResidualOutput(const InputParameters & parameters)
  : PetscOutput(parameters),
    _old_linear_norm(std::numeric_limits<Real>::max()),
    _old_nonlinear_norm(std::numeric_limits<Real>::max())
{
}

const char *
SPFIResidualOutput::ansi(const char * code)
{
  return Moose::colorConsole() ? code : "";
}

std::string
SPFIResidualOutput::coloredNorm(Real old_norm, Real norm, unsigned int precision)
{
  const char * highlight = RESIDUAL_GOOD_HIGHLIGHT;
  if (!std::isfinite(norm) || norm > old_norm)
    highlight = RESIDUAL_BAD_HIGHLIGHT;
  else if ((old_norm - norm) / old_norm <= 0.05)
    highlight = RESIDUAL_SLOW_HIGHLIGHT;

  std::ostringstream oss;
  oss << ansi(highlight) << ' ' << std::scientific << std::setprecision(precision) << norm << ' '
      << ansi(RESIDUAL_RESET);
  return oss.str();
}

void
SPFIResidualOutput::output()
{
  if (_current_execute_flag == EXEC_NONLINEAR)
  {
    if (_nonlinear_iter == 0)
      _old_nonlinear_norm = std::numeric_limits<Real>::max();

    _console << ansi(RESIDUAL_BRIGHT_MAGENTA) << std::right << std::setw(2) << _nonlinear_iter
             << " Nonlinear |R| = " << ansi(RESIDUAL_RESET)
             << coloredNorm(_old_nonlinear_norm, _norm) << '\n';
    _old_nonlinear_norm = _norm;
  }
  else if (_current_execute_flag == EXEC_LINEAR)
  {
    if (_linear_iter == 0)
      _old_linear_norm = std::numeric_limits<Real>::max();

    _console << ansi(RESIDUAL_BRIGHT_BLUE) << std::right << std::setw(7) << _linear_iter
             << " Linear |R| = " << ansi(RESIDUAL_RESET) << coloredNorm(_old_linear_norm, _norm)
             << '\n';
    _old_linear_norm = _norm;
  }

  _console << std::flush;
}
