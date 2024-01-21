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
#include "winx_globals.hpp"
#include "winx_platform.hpp"
#include "winx_util.hpp"

extern bool IS_DEBUG_MODE_ENABLED;
extern unsigned int src_polyfills_Winx_js_len;
extern unsigned char src_polyfills_Winx_js[];

std::string GetEmbeddedPolyfillData() {
  char* poly_data =
      (char*)malloc(sizeof(char) * (src_polyfills_Winx_js_len + 1));
  for (int i = 0; i < src_polyfills_Winx_js_len; i++) {
    poly_data[i] = src_polyfills_Winx_js[i];
  }
  poly_data[src_polyfills_Winx_js_len] = '\0';
  return std::string(poly_data);
}

void EmbeddedRequestGetterAccessor(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info);

class WinxEngine {
 private:
  std::string program_file;
  std::string program_emebdded_request;
  std::string external_polyfill_bootstrapper;

  v8::Isolate* isolate;
  v8::Isolate::CreateParams create_params;
  std::unique_ptr<Winx::WinxPlatform> platform;
  v8::Local<v8::Context> context;

  void InitializeEngine() {
    // argv[0]
    v8::V8::InitializeICUDefaultLocation(nullptr);
    v8::V8::InitializeExternalStartupData(nullptr);
    this->platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(this->platform.get());
    v8::V8::SetFlagsFromString(WinxConfig::get_v8_flags().c_str());
    v8::V8::Initialize();

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    this->create_params = create_params;
    this->isolate = isolate;
  }

  void ExecuteScript() {
    v8::Context::Scope context_scope(this->context);
    {
      v8::Local<v8::External> externalWinxEngine =
          v8::External::New(isolate, this);
      context->Global()
          ->Set(context,
                v8::String::NewFromUtf8(isolate, "winxEngine",
                                        v8::NewStringType::kNormal)
                    .ToLocalChecked(),
                externalWinxEngine)
          .FromJust();

      v8::Local<v8::String> initial_runtime;
      // TODO: Still really hacky :D
      if (this->external_polyfill_bootstrapper.empty()) {
        initial_runtime =
            v8::String::NewFromUtf8(isolate, GetEmbeddedPolyfillData().c_str(),
                                    v8::NewStringType::kNormal)
                .ToLocalChecked();
      } else {
        initial_runtime =
            v8::String::NewFromUtf8(
                isolate, this->external_polyfill_bootstrapper.c_str(),
                v8::NewStringType::kNormal)
                .ToLocalChecked();
      }
      v8::Local<v8::Script> initial_runtime_script =
          v8::Script::Compile(context, initial_runtime).ToLocalChecked();
      initial_runtime_script->Run(context).ToLocalChecked();

      v8::Local<v8::String> source_code =
          v8::String::NewFromUtf8(isolate, program_file.c_str(),
                                  v8::NewStringType::kNormal)
              .ToLocalChecked();
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source_code).ToLocalChecked();
      auto script_result = script->Run(context).ToLocalChecked();
      v8::String::Utf8Value utf8(isolate, script_result);
      // std::cout << "Result: " << *utf8 << std::endl;
    }
  }

 public:
  WinxEngine(std::string program_file,
             std::string program_embedded_request,
             std::string external_polyfill_bootstrapper) {
    this->program_file = program_file;
    this->program_emebdded_request = program_embedded_request;
    this->external_polyfill_bootstrapper = external_polyfill_bootstrapper;

    this->InitializeEngine();
  }

  std::string GetEmebeddedRequest() { return this->program_emebdded_request; }

  void RunProgram() {
    v8::Isolate* isolate = this->isolate;
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

    if (!this->program_emebdded_request.empty()) {
      global->SetAccessor(v8::String::NewFromUtf8(isolate, "request",
                                                  v8::NewStringType::kNormal)
                              .ToLocalChecked(),
                          EmbeddedRequestGetterAccessor);
    }

    v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
    this->context = context;
    this->ExecuteScript();
  }

  void DisposeEngine() {
    this->isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete this->create_params.array_buffer_allocator;
  }
};

void EmbeddedRequestGetterAccessor(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Local<v8::External> externalWinxEngine =
      info.This()
          ->Get(info.GetIsolate()->GetCurrentContext(),
                v8::String::NewFromUtf8(info.GetIsolate(), "winxEngine",
                                        v8::NewStringType::kNormal)
                    .ToLocalChecked())
          .ToLocalChecked()
          .As<v8::External>();
  WinxEngine* winxEngine =
      static_cast<WinxEngine*>(externalWinxEngine->Value());

  // Access the isolate and context
  v8::Isolate* isolate = info.GetIsolate();
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  // Parse the JSON string
  v8::Local<v8::String> request =
      v8::String::NewFromUtf8(isolate,
                              winxEngine->GetEmebeddedRequest().c_str(),
                              v8::NewStringType::kNormal)
          .ToLocalChecked();
  v8::Local<v8::Value> json =
      v8::JSON::Parse(context, request).ToLocalChecked();

  // Set the property in the object
  info.GetReturnValue().Set(json);
}

int internal_main(int argc, char* argv[]) {
  std::string filename;
  std::string environment_embedded_request;

  CLI::App app{"Winx - A V8-based JavaScript runtime for Macintosh."};
  app.add_flag("-D,--debug", IS_DEBUG_MODE_ENABLED,
               "Enable debug mode (default: false)")
      ->default_val(false);
  app.add_option("-r,--request", environment_embedded_request,
                 "A JSON to pass the runtime");
  app.add_option("filename", filename, "The program needed to execute")
      ->required();
  CLI11_PARSE(app, argc, argv);

  std::string program_file = Winx::Util::read_file(filename);
  std::string bootstrapper =
      Winx::Util::read_file(WinxConfig::get_winx_flag("polyfills_file"));

  WinxEngine engine(program_file, environment_embedded_request, bootstrapper);
  engine.RunProgram();
  engine.DisposeEngine();
  return 0;
}

int main(int argc, char* argv[]) {
  internal_main(argc, argv);
}

#endif  // WINX_MAIN
