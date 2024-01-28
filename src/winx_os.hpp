#ifndef __SRC_WINX_OS_HPP__
#define __SRC_WINX_OS_HPP__

#include "util.hpp"
#include "winx_binding.hpp"
#include <iostream>
#include <uv.h>
#include <v8.h>

namespace Winx::Bindings::Os
{

static void get_free_memory(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    CHECK_EQ(args.Length(), 0);
    auto freeMemory = static_cast<double>(uv_get_free_memory());
    args.GetReturnValue().Set(freeMemory);
}

static void get_total_memory(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    CHECK_EQ(args.Length(), 0);
    auto totalMemory = static_cast<double>(uv_get_total_memory());
    args.GetReturnValue().Set(totalMemory);
}

static void get_cpu_info(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    CHECK_EQ(args.Length(), 0);
    v8::Isolate *isolate = args.GetIsolate();
    uv_cpu_info_t *cpu_info;
    int num_cpus;
    uv_cpu_info(&cpu_info, &num_cpus);
    v8::Local<v8::Array> cpu_info_array = v8::Array::New(isolate, num_cpus);

    // TODO: Refactor this to be less verbose
    // https://github.com/nodejs/node/blob/64c6d97463c29bade4d6081683dab2cd7cda298d/src/node_os.cc#L111
    // Node directly sends a vector and parses in JS-land rather than doing this Object::Set nonsense
    for (int i = 0; i < num_cpus; i++)
    {
        v8::Local<v8::Object> cpu_info_object = v8::Object::New(isolate);
        cpu_info_object
            ->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "model"),
                  GEN_STRING_UTF8(isolate, cpu_info[i].model))
            .ToChecked();
        cpu_info_object
            ->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "speed"),
                  v8::Number::New(isolate, cpu_info[i].speed))
            .ToChecked();

        auto times = v8::Object::New(isolate);
        times
            ->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "user"),
                  v8::Number::New(isolate, cpu_info[i].cpu_times.user))
            .ToChecked();
        times
            ->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "sys"),
                  v8::Number::New(isolate, cpu_info[i].cpu_times.sys))
            .ToChecked();
        times
            ->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "nice"),
                  v8::Number::New(isolate, cpu_info[i].cpu_times.nice))
            .ToChecked();
        times
            ->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "irq"),
                  v8::Number::New(isolate, cpu_info[i].cpu_times.irq))
            .ToChecked();
        times
            ->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "idle"),
                  v8::Number::New(isolate, cpu_info[i].cpu_times.idle))
            .ToChecked();
        cpu_info_object->Set(isolate->GetCurrentContext(), GEN_STRING_UTF8(isolate, "times"), times).ToChecked();

        cpu_info_array->Set(isolate->GetCurrentContext(), i, cpu_info_object).ToChecked();
    }

    uv_free_cpu_info(cpu_info, num_cpus);
    args.GetReturnValue().Set(cpu_info_array);
}

static void prompt(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() > 1)
    {
        return;
    }
    v8::Isolate *isolate = args.GetIsolate();

    if (args.Length() == 1)
    {
        v8::String::Utf8Value prompt_prefix(isolate, args[0]);
        std::cout << *prompt_prefix;
    }

    std::string prompt_data;
    std::getline(std::cin, prompt_data);

    args.GetReturnValue().Set(
        v8::String::NewFromUtf8(isolate, prompt_data.c_str(), v8::NewStringType::kNormal).ToLocalChecked());
}

v8::Local<v8::ObjectTemplate> EngineBind(v8::Isolate *isolate)
{
    v8::Local<v8::ObjectTemplate> os = Winx::Binding::create_winx_object_binding(isolate);
    Winx::Binding::create_winx_function_binding(isolate, os, "get_free_memory", get_free_memory);
    Winx::Binding::create_winx_function_binding(isolate, os, "get_total_memory", get_total_memory);
    Winx::Binding::create_winx_function_binding(isolate, os, "stdin", prompt);
    Winx::Binding::create_winx_function_binding(isolate, os, "cpus", get_cpu_info);

    // TODO: Remove this and implement in JS-land; demoing adding attributes instead of functions
    // Also remember that windows has different EOL \r\n
    os->Set(isolate, "EOL", GEN_STRING_UTF8(isolate, "\n"), PropertyAttribute::ReadOnly);
    return os;
}

} // namespace Winx::Bindings::Os

#endif // __SRC_WINX_OS_HPP__