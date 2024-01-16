#ifndef UTIL_HPP
#define UTIL_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

// really simple debug control, going to make this "better" one day
#define K_DEBUG_ENABLED true

namespace Winx::Util {
std::string* read_file(std::string source);
void debug_print(std::string prefix, std::string message);
}  // namespace Winx::Util

#endif  // UTIL_HPP