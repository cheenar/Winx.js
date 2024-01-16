#include "winx_console.hpp"

namespace Winx::Bindings::Console {

void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  formatted_print(args, "Winx 🦋");
}

void debug(const v8::FunctionCallbackInfo<v8::Value>& args) {
  formatted_print(args, "debug");
}

}  // namespace Winx::Bindings::Console

static void formatted_print(const v8::FunctionCallbackInfo<v8::Value>& args,
                            std::string prefix) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1) {
    return;
  }
  v8::String::Utf8Value message(isolate, args[0]);
  if (prefix == "") {
    std::cout << *message << std::endl;
  } else {
    std::cout << "[" << termcolor::bold << termcolor::magenta << prefix
              << termcolor::reset << "] " << *message << std::endl;
  }
}
