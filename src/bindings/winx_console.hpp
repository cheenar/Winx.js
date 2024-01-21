#ifndef WINX_CONSOLE_HPP
#define WINX_CONSOLE_HPP

#include <v8.h>

#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

#include "winx_binding.hpp"

namespace Winx::Bindings::Console {

void log(const v8::FunctionCallbackInfo<v8::Value>& args);
void debug(const v8::FunctionCallbackInfo<v8::Value>& args);

v8::Local<v8::ObjectTemplate> EngineBind(v8::Isolate* isolate);

}  // namespace Winx::Bindings::Console

#endif  // WINX_CONSOLE_HPP