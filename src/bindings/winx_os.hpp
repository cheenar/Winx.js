#ifndef WINX_OS_HPP
#define WINX_OS_HPP

#include <uv.h>
#include <v8.h>
#include <iostream>

namespace Winx::Bindings::Os {

static void get_free_memory(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() > 0) {
    return;
  }
  auto freeMemory = static_cast<double>(uv_get_free_memory());
  args.GetReturnValue().Set(freeMemory);
}

static void get_total_memory(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() > 0) {
    return;
  }
  auto totalMemory = static_cast<double>(uv_get_total_memory());
  args.GetReturnValue().Set(totalMemory);
}

static void prompt(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() > 1) {
    return;
  }
  v8::Isolate* isolate = args.GetIsolate();

  if (args.Length() == 1) {
    v8::String::Utf8Value prompt_prefix(isolate, args[0]);
    std::cout << *prompt_prefix;
  }

  std::string prompt_data;
  std::getline(std::cin, prompt_data);

  args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate,
                                                    prompt_data.c_str(),
                                                    v8::NewStringType::kNormal)
                                .ToLocalChecked());
}

}  // namespace Winx::Bindings::Os

#endif  // WINX_OS_HPP