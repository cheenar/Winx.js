#include "util.hpp"

namespace Util {

std::string* read_file(std::string source)
{
    std::ifstream file_stream(source.c_str());
    std::string* raw = new std::string((std::istreambuf_iterator<char>(file_stream)),
        (std::istreambuf_iterator<char>()));
    return raw;
}

} // namespace Util