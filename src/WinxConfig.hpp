#ifndef __WINX_CONFIG_HPP__
#define __WINX_CONFIG_HPP__

#include <toml.h>

#include <iostream>
#include <string>
#include "util.hpp"

using namespace std;

namespace WinxConfig {
string get_v8_flags(string config_file = "Winx.toml");
}  // namespace WinxConfig

#endif  // __WINX_CONFIG_HPP__