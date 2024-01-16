#ifndef WINXCONFIG_CPP
#define WINXCONFIG_CPP

#include "winx_config.hpp"

namespace WinxConfig {

toml::table parse_config(string config_file) {
  toml::table tbl;
  try {
    tbl = toml::parse_file(config_file);
  } catch (const toml::parse_error& err) {
    cerr << "Parsing failed:\n" << err << "\n";
    exit(1);
  }
  return tbl;
}

string get_v8_flags(string config_file) {
  auto tbl = parse_config(config_file);
  auto flags = tbl["v8"];
  if (!flags.is_table()) {
    Winx::Util::debug_print("V8 Flags", "no flags detected in " + config_file);
    return "";
  }
  string flag_str;
  auto v8_flags = flags.as_table();
  for (auto it = v8_flags->begin(); it != v8_flags->end(); ++it) {
    if (it->second.is<string>()) {
      flag_str += "--" + string(it->first.str()) + "=" +
                  *it->second.value<string>() + " ";
    }
    if (it->second.is<bool>()) {
      auto v8_flag_bool_prefix = *it->second.value<bool>() ? "" : "no-";
      flag_str +=
          "--" + string(v8_flag_bool_prefix) + string(it->first.str()) + " ";
    }
    if (it->second.is<int64_t>()) {
      flag_str += "--" + string(it->first.str()) + "=" +
                  to_string(*it->second.value<int64_t>()) + " ";
    }
  }
  Winx::Util::debug_print("V8 Flags", flag_str);
  return flag_str;
}

string get_winx_flag(string flag_name, string config_file) {
  auto tbl = parse_config(config_file);
  auto config_value = tbl["winx"][flag_name].value_or(""s);
  if (config_value != "") {
    Winx::Util::debug_print("Winx Flag", flag_name + "=" + config_value);
  }
  return config_value;
}

}  // namespace WinxConfig

#endif  // WINXCONFIG_CPP