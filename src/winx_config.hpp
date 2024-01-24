#ifndef __WINX_CONFIG_HPP__
#define __WINX_CONFIG_HPP__

#include <toml.h>

#include "winx_globals.hpp"
#include "winx_util.hpp"
#include <iostream>
#include <string>

#define WINX_CONFIG_POLYFILLS "polyfills_file"
#define WINX_CONFIG_MODULE_FOLDER "modules_folder"

using namespace std;

namespace Winx::Config
{

/**
 * @brief Get the v8 flags object from the Winx.toml file.
 *
 * @param config_file The configuration file name.
 * @return string The v8 flags string.
 */
string get_v8_flags(string config_file = "Winx.toml");

/**
 * @brief Get the winx flag object from the Winx.toml file.
 *
 * @param flag_name The configuration flag name.
 * @param config_file The configuration file name.
 * @return string The winx flag.
 */
string get_winx_flag(string flag_name, string default_value = "", string config_file = "Winx.toml");

} // namespace Winx::Config

#endif // __WINX_CONFIG_HPP__