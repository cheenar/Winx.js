#ifndef WINX_MAIN
#define WINX_MAIN

#include <libplatform/libplatform.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <toml.h>
#include <v8.h>

#include <cli11/CLI11.hpp>
#include "bindings/winx_console.hpp"
#include "bindings/winx_fs.hpp"
#include "bindings/winx_os.hpp"
#include "embedded_winx_polyfill.h"
#include "winx_config.hpp"
#include "winx_platform.hpp"
#include "winx_util.hpp"

bool kIsDebugModeEnabled;

int main(int argc, char* argv[]) {
  std::string filename;

  CLI::App app{"Winx - A V8-based JavaScript runtime for Macintosh."};
  bool IS_DEBUG;
  app.add_flag("-D,--debug", IS_DEBUG, "Enable debug mode (default: false)")
      ->default_val(false);
  app.add_option("filename", filename, "The program needed to execute")
      ->required();
  CLI11_PARSE(app, argc, argv);
  kIsDebugModeEnabled = IS_DEBUG;

  // Read program to run
  std::string* source_file = Winx::Util::read_file(filename);

  std::string* bootstrapper =
      Winx::Util::read_file(WinxConfig::get_winx_flag("polyfills_file"));

  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<Winx::WinxPlatform> platform =
      v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());

  v8::V8::SetFlagsFromString(WinxConfig::get_v8_flags().c_str());

  v8::V8::Initialize();

  // Create a new Isolate and make it the current one.
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();

  v8::Isolate* isolate = v8::Isolate::New(create_params);
  {
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    // Create a new context.
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // create a prototype method on the global
    v8::Local<v8::ObjectTemplate> winx = v8::ObjectTemplate::New(isolate);

    v8::Local<v8::ObjectTemplate> console = v8::ObjectTemplate::New(isolate);
    v8::Local<v8::ObjectTemplate> os = v8::ObjectTemplate::New(isolate);

    console->Set(
        isolate, "log",
        v8::FunctionTemplate::New(isolate, Winx::Bindings::Console::log));
    console->Set(
        isolate, "debug",
        v8::FunctionTemplate::New(isolate, Winx::Bindings::Console::debug));

    winx->Set(isolate, "readFile",
              v8::FunctionTemplate::New(isolate,
                                        Winx::Bindings::FileSystem::read_file));

    os->Set(isolate, "getFreeMemory",
            v8::FunctionTemplate::New(isolate,
                                      Winx::Bindings::Os::get_free_memory));

    os->Set(isolate, "getTotalMemory",
            v8::FunctionTemplate::New(isolate,
                                      Winx::Bindings::Os::get_total_memory));
    os->Set(isolate, "stdin",
            v8::FunctionTemplate::New(isolate, Winx::Bindings::Os::prompt));

    winx->Set(isolate, "console", console);
    winx->Set(isolate, "os", os);
    global->Set(isolate, "Winx", winx);

    v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);
    {
      v8::Local<v8::String> initial_runtime;
      // TODO: Still really hacky :D
      if (*bootstrapper != " ") {
        initial_runtime =
            v8::String::NewFromUtf8(isolate, (const char*)src_polyfills_Winx_js,
                                    v8::NewStringType::kNormal)
                .ToLocalChecked();
      } else {
        initial_runtime =
            v8::String::NewFromUtf8(isolate, bootstrapper->c_str(),
                                    v8::NewStringType::kNormal)
                .ToLocalChecked();
      }
      v8::Local<v8::Script> initial_runtime_script =
          v8::Script::Compile(context, initial_runtime).ToLocalChecked();
      initial_runtime_script->Run(context).ToLocalChecked();

      v8::Local<v8::String> source_code =
          v8::String::NewFromUtf8(isolate, source_file->c_str(),
                                  v8::NewStringType::kNormal)
              .ToLocalChecked();
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source_code).ToLocalChecked();
      /* v8::Local<v8::Value> result =  */
      script->Run(context).ToLocalChecked();
      // v8::String::Utf8Value utf8(isolate, result);
      // std::cout << "Result: " << *utf8 << std::endl;
    }
  }

  // clear source(s)
  delete source_file;
  delete bootstrapper;

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::DisposePlatform();
  delete create_params.array_buffer_allocator;
  return 0;
}

#endif  // WINX_MAIN