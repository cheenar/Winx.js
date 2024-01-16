#ifndef WINX_CONSOLE_HPP
#define WINX_CONSOLE_HPP

#include <v8.h>
#include <iostream>

static void formatted_print(const v8::FunctionCallbackInfo<v8::Value>& args,
                            const char* prefix);

namespace Winx::Bindings::Console {
void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  formatted_print(args, "");
}
void debug(const v8::FunctionCallbackInfo<v8::Value>& args) {
  formatted_print(args, "\033[0;m[\033[1;35mdebug\033[0;m] ");
}
}  // namespace Winx::Bindings::Console

static void formatted_print(const v8::FunctionCallbackInfo<v8::Value>& args,
                            const char* prefix) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1) {
    return;
  }
  v8::String::Utf8Value message(isolate, args[0]);
  std::cout << prefix << *message << std::endl;
}

#endif  // WINX_CONSOLE_HPP