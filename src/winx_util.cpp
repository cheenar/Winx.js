#include "winx_util.hpp"

namespace Winx::Util {

std::string read_file(std::string source) {
  std::ifstream file_stream(source.c_str());
  std::string raw((std::istreambuf_iterator<char>(file_stream)),
                  (std::istreambuf_iterator<char>()));
  return raw;
}

void write_file(std::string file_name, std::string data) {
  std::ofstream file_stream(file_name.c_str());
  file_stream << data;
  file_stream.close();
}

void debug_print(std::string prefix, std::string message) {
  if (!IS_DEBUG_MODE_ENABLED) {
    return;
  }
  std::cout << "[" << termcolor::bold << termcolor::magenta << prefix
            << termcolor::reset << "]: " << message << std::endl;
}

}  // namespace Winx::Util