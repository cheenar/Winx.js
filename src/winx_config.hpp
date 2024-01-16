#ifndef __WINX_CONFIG_HPP__
#define __WINX_CONFIG_HPP__

#include <toml.h>

#include <iostream>
#include <string>
#include "winx_util.hpp"

using namespace std;

namespace WinxConfig {
string get_v8_flags(string config_file = "Winx.toml");
string get_winx_flag(string flag_name, string config_file = "Winx.toml");
}  // namespace WinxConfig

#endif  // __WINX_CONFIG_HPP__