#ifndef WINX_OS_HPP
#define WINX_OS_HPP

#include <uv.h>
#include <v8.h>

namespace Winx::Bindings::Os {

static void get_free_memory(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() > 0) {
        return;
    }
    auto freeMemory = static_cast<double>(uv_get_free_memory());
    args.GetReturnValue().Set(freeMemory);
}

static void get_total_memory(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() > 0) {
        return;
    }
    auto totalMemory = static_cast<double>(uv_get_total_memory());
    args.GetReturnValue().Set(totalMemory);
}

} // namespace Winx::Bindings::Os

#endif // WINX_OS_HPP