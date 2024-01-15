#ifndef WINXCONFIG_CPP
#define WINXCONFIG_CPP

#include "WinxConfig.hpp"

namespace WinxConfig {

toml::table parse_config(string config_file) {
  toml::table tbl;
  try {
    tbl = toml::parse_file(config_file);
    cout << tbl;
  } catch (const toml::parse_error& err) {
    cerr << "Parsing failed:\n" << err << "\n";
    exit(1);
  }
  return tbl;
}

}  // namespace WinxConfig

#endif  // WINXCONFIG_CPP