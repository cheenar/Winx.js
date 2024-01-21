#ifndef WINX_CONSOLE_HPP
#define WINX_CONSOLE_HPP

#include <v8.h>

#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

#include "winx_binding.hpp"

static void formatted_print(const v8::FunctionCallbackInfo<v8::Value>& args,
                            std::string prefix);

namespace Winx::Bindings::Console {

void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  formatted_print(args, "log");
}

void debug(const v8::FunctionCallbackInfo<v8::Value>& args) {
  formatted_print(args, "debug");
}

v8::Local<v8::ObjectTemplate> EngineBind(v8::Isolate* isolate) {
  v8::Local<v8::ObjectTemplate> console = create_winx_object_binding(isolate);
  create_winx_function_binding(isolate, console, "log", log);
  create_winx_function_binding(isolate, console, "debug", debug);
  return console;
}

}  // namespace Winx::Bindings::Console

static void formatted_print(const v8::FunctionCallbackInfo<v8::Value>& args,
                            std::string prefix) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1) {
    return;
  }
  v8::String::Utf8Value message(isolate, args[0]);

  if (prefix.empty()) {
    std::cout << *message << std::endl;
  } else {
    std::cout << "[" << termcolor::magenta << termcolor::bold << prefix
              << termcolor::reset << "] " << *message << std::endl;
  }
}

#endif  // WINX_CONSOLE_HPP