#include "winx_util.hpp"

namespace Winx::Util {

std::string read_file(std::string source) {
  // std::ifstream file_stream(source.c_str());
  // std::string raw((std::istreambuf_iterator<char>(file_stream)),
  //                 (std::istreambuf_iterator<char>()));
  // return raw;
  using namespace std;
  ifstream file(source, ios::in | ios::binary | ios::ate);
  if (file.is_open()) {
    file.seekg(0, ios::end);
    int size = file.tellg();
    char* contents = new char[size];
    file.seekg(0, ios::beg);
    file.read(contents, size);
    file.close();
    return string(contents, size);
  }
  return "";
}

void write_file(std::string file_name, std::string data) {
  std::ofstream file_stream(file_name.c_str());
  file_stream << data;
  file_stream.close();
}

WinxFileHandle* open_file(std::string file_name, std::string mode) {
  WinxFileHandle* file_handle = new WinxFileHandle();

  if (mode.find("r") != std::string::npos) {
    file_handle->in = new std::ifstream(file_name.c_str());
  } else {
    file_handle->in = nullptr;
  }
  if (mode.find("w") != std::string::npos) {
    file_handle->out = new std::ofstream(file_name.c_str());
  } else {
    if (mode.find("a") != std::string::npos) {
      file_handle->out =
          new std::ofstream(file_name.c_str(), std::ios_base::app);
    } else {
      file_handle->out = nullptr;
    }
  }

  return file_handle;
}

void close_file(WinxFileHandle file_handle) {
  if (file_handle.in != nullptr) {
    file_handle.in->close();
    delete file_handle.in;
  }
  if (file_handle.out != nullptr) {
    file_handle.out->close();
    delete file_handle.out;
  }
}

std::string read_file(WinxFileHandle file_handle) {
  std::string raw;
  if (file_handle.in != nullptr) {
    raw = std::string((std::istreambuf_iterator<char>(*file_handle.in)),
                      (std::istreambuf_iterator<char>()));
  }
  return raw;
}

void write_file(WinxFileHandle file_handle, std::string data) {
  if (file_handle.out != nullptr) {
    *file_handle.out << data;
  }
}

void debug_print(std::string prefix, std::string message) {
  if (!IS_DEBUG_MODE_ENABLED) {
    return;
  }
  std::cout << "[" << termcolor::bold << termcolor::magenta << prefix
            << termcolor::reset << "]: " << message << std::endl;
}

}  // namespace Winx::Util