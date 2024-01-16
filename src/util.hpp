#ifndef UTIL_HPP
#define UTIL_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

namespace Util {
std::string* read_file(std::string source);
void debug_print(std::string prefix, std::string message);
}  // namespace Util

#endif  // UTIL_HPP