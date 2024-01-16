#ifndef UTIL_HPP
#define UTIL_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

namespace WinxUtil {
std::string* read_file(std::string source);
void debug_print(std::string prefix, std::string message);
}  // namespace WinxUtil

#endif  // UTIL_HPP