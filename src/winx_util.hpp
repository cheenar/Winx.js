#ifndef UTIL_HPP
#define UTIL_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

#include "winx_globals.hpp"

namespace Winx::Util
{
typedef struct
{
    std::ifstream *in;
    std::ofstream *out;
} WinxFileHandle;

std::string read_file(std::string source);
void write_file(std::string file_name, std::string data);

WinxFileHandle *open_file(std::string file_name, std::string mode);
void close_file(WinxFileHandle file_handle);

std::string read_file(WinxFileHandle file_handle);
void write_file(WinxFileHandle file_handle, std::string data);

void debug_print(std::string prefix, std::string message);
} // namespace Winx::Util

#endif // UTIL_HPP