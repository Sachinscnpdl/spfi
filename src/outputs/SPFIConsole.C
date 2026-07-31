#include "SPFIConsole.h"

#include "Moose.h"

#include <iomanip>
#include <sstream>

registerMooseObject("spfiApp", SPFIConsole);

namespace
{
constexpr const char * RESET = "\033[0m";
constexpr const char * BRIGHT_CYAN = "\033[96m";
}

InputParameters
SPFIConsole::validParams()
{
  InputParameters params = Console::validParams();
  params.addClassDescription(
      "SPFI console output with colored timestep and postprocessor headings.");
  return params;
}

SPFIConsole::SPFIConsole(const InputParameters & parameters) : Console(parameters) {}

const char *
SPFIConsole::ansi(const char * code)
{
  return Moose::colorConsole() ? code : "";
}

void
SPFIConsole::timestepSetup()
{
  if (onInterval())
    writeSPFITimestepInformation(/* output_dt = */ true);
  else
    _console << ansi(BRIGHT_CYAN) << "Time Step " << timeStep() << ansi(RESET) << '\n'
             << std::flush;
}

void
SPFIConsole::writeSPFITimestepInformation(bool output_dt)
{
  if (!_transient)
    return;

  std::ostringstream oss;
  oss << '\n' << ansi(BRIGHT_CYAN) << "Time Step " << timeStep();
  const unsigned int time_step_digits =
      oss.str().length() - std::string(ansi(BRIGHT_CYAN)).length() - 11;
  oss << ", time = " << formatTime(getOutputTime());

  if (output_dt)
  {
    if (!_verbose)
      oss << ", dt = " << std::left << formatTime(dt());
    else
    {
      const unsigned int fillsize = 19 + time_step_digits;
      oss << '\n'
          << std::right << std::setw(fillsize) << std::setfill(' ') << "old time = " << std::left
          << formatTime(timeOld()) << '\n'
          << std::right << std::setw(fillsize) << std::setfill(' ') << "dt = " << std::left
          << formatTime(dt()) << '\n'
          << std::right << std::setw(fillsize) << std::setfill(' ')
          << "old dt = " << formatTime(_dt_old);
    }
  }

  oss << ansi(RESET) << '\n';
  _console << oss.str() << std::flush;
}

void
SPFIConsole::outputPostprocessors()
{
  TableOutput::outputPostprocessors();

  if (!_postprocessor_table.empty())
  {
    std::ostringstream oss;
    oss << '\n' << ansi(BRIGHT_CYAN) << "Postprocessor Values:" << ansi(RESET) << '\n';
    _postprocessor_table.sortColumns();
    _postprocessor_table.printTable(oss, _max_rows, _fit_mode);
    _console << oss.str() << std::endl;
  }
}
