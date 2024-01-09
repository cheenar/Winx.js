#ifndef WINX_OS_HPP
#define WINX_OS_HPP

#include "../../include/libuv/uv.h"
#include "../../include/v8/v8.h"

namespace Winx::Bindings::Os {

void get_free_memory(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() > 0) {
      return;
    }
    auto freeMemory = static_cast<double>(uv_get_free_memory());
    args.GetReturnValue().Set(freeMemory);
}

void get_total_memory(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() > 0) {
      return;
    }
    auto totalMemory = static_cast<double>(uv_get_total_memory());
    args.GetReturnValue().Set(totalMemory);
}

}


#endif  // WINX_OS_HPP