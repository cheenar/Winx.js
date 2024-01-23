#ifndef __SRC_WINX_CONSOLE_HPP__
#define __SRC_WINX_CONSOLE_HPP__

#include <v8.h>

#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

#include "winx_binding.hpp"
#include "winx_globals.hpp"

/**
 * @brief Produces a formatted print output with debug flags enabled.
 *
 * @param args The function callback args.
 * @param prefix The prefix to print before the message.
 */
static void formatted_print(const v8::FunctionCallbackInfo<v8::Value> &args, std::string prefix);

namespace Winx::Bindings::Console
{

/**
 * @brief Produces a stdout message.
 *
 * @param args The function callback args.
 */
void log(const v8::FunctionCallbackInfo<v8::Value> &args);

/**
 * @brief Produces a stdout message with debug headers.
 *
 * @param args The function callback args.
 */
void debug(const v8::FunctionCallbackInfo<v8::Value> &args);

/**
 * @brief Binds the methods to the V8 engine.
 *
 * @param isolate The v8 isolate.
 * @return v8::Local<v8::ObjectTemplate> The object template to bind to the engine.
 */
v8::Local<v8::ObjectTemplate> EngineBind(v8::Isolate *isolate);

} // namespace Winx::Bindings::Console

static void formatted_print(const v8::FunctionCallbackInfo<v8::Value> &args, std::string prefix)
{
    v8::Isolate *isolate = args.GetIsolate();
    if (args.Length() < 1)
    {
        return;
    }
    v8::String::Utf8Value message(isolate, args[0]);

    if (prefix.empty() || !IS_DEBUG_MODE_ENABLED)
    {
        std::cout << *message << std::endl;
    }
    else
    {
        std::cout << "[" << termcolor::magenta << termcolor::bold << prefix << termcolor::reset << "] " << *message
                  << std::endl;
    }
}

void Winx::Bindings::Console::log(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    formatted_print(args, "log");
}

void Winx::Bindings::Console::debug(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    formatted_print(args, "debug");
}

v8::Local<v8::ObjectTemplate> Winx::Bindings::Console::EngineBind(v8::Isolate *isolate)
{
    v8::Local<v8::ObjectTemplate> console = Winx::Binding::create_winx_object_binding(isolate);
    Winx::Binding::create_winx_function_binding(isolate, console, "log", log);
    Winx::Binding::create_winx_function_binding(isolate, console, "debug", debug);
    return console;
}

#endif // __SRC_WINX_CONSOLE_HPP__