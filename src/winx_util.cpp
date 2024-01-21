#include "winx_util.hpp"

namespace Winx::Util {

std::string read_file(std::string source) {
  std::ifstream file_stream(source.c_str());
  std::string raw((std::istreambuf_iterator<char>(file_stream)),
                  (std::istreambuf_iterator<char>()));
  return raw;
}

void debug_print(std::string prefix, std::string message) {
  if (!kIsDebugModeEnabled)
    return;
  std::cout << "[" << termcolor::bold << termcolor::magenta << prefix
            << termcolor::reset << "]: " << message << std::endl;
}

}  // namespace Winx::Util