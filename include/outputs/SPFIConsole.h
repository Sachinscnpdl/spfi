#pragma once

#include "Console.h"

/**
 * SPFI-specific screen output with colored timestep and postprocessor headings.
 */
class SPFIConsole : public Console
{
public:
  static InputParameters validParams();

  SPFIConsole(const InputParameters & parameters);

  void timestepSetup() override;

protected:
  void outputPostprocessors() override;

private:
  void writeSPFITimestepInformation(bool output_dt);

  static const char * ansi(const char * code);
};
