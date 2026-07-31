#pragma once

#include "Moose.h"

#include <iostream>

namespace SPFIConsoleUtils
{
inline const char *
ansi(const char * code)
{
  return Moose::colorConsole() ? code : "";
}

inline void
writeBanner()
{
  constexpr const char * reset = "\033[0m";
  constexpr const char * bright_green = "\033[92m";
  constexpr const char * bright_yellow = "\033[93m";
  constexpr const char * bright_magenta = "\033[95m";
  constexpr const char * bright_cyan = "\033[96m";

  std::cout << '\n'
            << ansi(bright_cyan) << "  ✦********************************************************✦\n"
            << "     " << ansi(bright_magenta) << "******" << ansi(bright_cyan) << "  ******"
            << ansi(bright_green) << "  ******" << ansi(bright_yellow) << "  ******\n"
            << "     " << ansi(bright_magenta) << "**    " << ansi(bright_cyan) << "  **  **"
            << ansi(bright_green) << "  **    " << ansi(bright_yellow) << "    **  \n"
            << "     " << ansi(bright_magenta) << "******" << ansi(bright_cyan) << "  ******"
            << ansi(bright_green) << "  ***** " << ansi(bright_yellow) << "    **  \n"
            << "     " << ansi(bright_magenta) << "    **" << ansi(bright_cyan) << "  **    "
            << ansi(bright_green) << "  **    " << ansi(bright_yellow) << "    **  \n"
            << "     " << ansi(bright_magenta) << "******" << ansi(bright_cyan) << "  **    "
            << ansi(bright_green) << "  **    " << ansi(bright_yellow) << "  ******\n"
            << '\n'
            << ansi(bright_cyan) << "                 ◆ Multi-Physics Simulation ◆\n"
            << "  ✦********************************************************✦" << ansi(reset)
            << "\n\n"
            << std::flush;
}
}
