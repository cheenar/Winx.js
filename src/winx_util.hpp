#ifndef UTIL_HPP
#define UTIL_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

#include "winx_globals.hpp"

extern bool IS_DEBUG_MODE_ENABLED;

namespace Winx::Util {
std::string read_file(std::string source);
void debug_print(std::string prefix, std::string message);
}  // namespace Winx::Util

#endif  // UTIL_HPP