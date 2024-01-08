#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/v8/libplatform/libplatform.h"
#include "../include/v8/v8.h"

#include <fstream>
#include <iostream>
#include <string>

namespace Winx {}
namespace Winx::Bindings {}
namespace Winx::Bindings::Console {
void log(const v8::FunctionCallbackInfo<v8::Value>& args);
void debug(const v8::FunctionCallbackInfo<v8::Value>& args);
void formatted_print(const v8::FunctionCallbackInfo<v8::Value>& args,
                     const char* prefix);
}  // namespace Winx::Bindings::Console

void Winx::Bindings::Console::formatted_print(
    const v8::FunctionCallbackInfo<v8::Value>& args,
    const char* prefix) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1) {
    return;
  }
  v8::String::Utf8Value message(isolate, args[0]);
  std::cout << prefix << *message << std::endl;
}

void Winx::Bindings::Console::log(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  Winx::Bindings::Console::formatted_print(args, "");
}

void Winx::Bindings::Console::debug(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  Winx::Bindings::Console::formatted_print(args, "\033[0;m[\033[1;35mdebug\033[0;m] ");
}

std::string* readFile(std::string source) {
  std::ifstream file_stream(source.c_str());
  std::string* raw =
      new std::string((std::istreambuf_iterator<char>(file_stream)),
                      (std::istreambuf_iterator<char>()));
  return raw;
}

int main(int argc, char* argv[]) {
  // Read program to run
  std::string* source_file = readFile(std::string(argv[1]));

  // TODO: this will never be portable!
  std::string* bootstrapper = readFile(std::string("./winx-polyfills.js"));

  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
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
    winx->Set(isolate, "log",
              v8::FunctionTemplate::New(isolate, Winx::Bindings::Console::log));
    winx->Set(isolate, "debug",
              v8::FunctionTemplate::New(isolate, Winx::Bindings::Console::debug));
    global->Set(isolate, "Winx", winx);

    v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);
    {
      v8::Local<v8::String> initial_runtime =
          v8::String::NewFromUtf8(isolate, bootstrapper->c_str(),
                                  v8::NewStringType::kNormal)
              .ToLocalChecked();
      v8::Local<v8::Script> initial_runtime_script =
          v8::Script::Compile(context, initial_runtime).ToLocalChecked();
      initial_runtime_script->Run(context).ToLocalChecked();

      v8::Local<v8::String> source_code =
          v8::String::NewFromUtf8(isolate, source_file->c_str(),
                                  v8::NewStringType::kNormal)
              .ToLocalChecked();
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source_code).ToLocalChecked();
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
      v8::String::Utf8Value utf8(isolate, result);
      std::cout << "Result: " << *utf8 << std::endl;
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