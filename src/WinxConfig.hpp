#ifndef __WINX_CONFIG_HPP__
#define __WINX_CONFIG_HPP__

#include <toml.h>

#include <iostream>
#include <string>

using namespace std;

namespace WinxConfig {
toml::table parse_config(string config_file = "Winx.toml");
}  // namespace WinxConfig

#endif  // __WINX_CONFIG_HPP__