#ifndef WINX_MAIN
#define WINX_MAIN

#include "winx.hpp"

namespace Winx
{

void WinxEngine::InitializeEngine()
{
    // argv[0]
    V8::InitializeICUDefaultLocation(nullptr);
    V8::InitializeExternalStartupData(nullptr);
    this->platform = platform::NewDefaultPlatform();
    V8::InitializePlatform(this->platform.get());
    V8::SetFlagsFromString(WinxConfig::get_v8_flags().c_str());
    V8::Initialize();

    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate *isolate = Isolate::New(create_params);

    isolate->SetHostImportModuleDynamicallyCallback(Winx::Modules::ResolveDynamicModuleCallback);

    this->create_params = create_params;
    this->isolate = isolate;
}

void WinxEngine::ExecuteScript()
{
    Context::Scope context_scope(this->context);
    {
        Local<External> externalWinxEngine = External::New(isolate, this);
        context->Global()
            ->Set(context, String::NewFromUtf8(isolate, "winxEngine", NewStringType::kNormal).ToLocalChecked(),
                  externalWinxEngine)
            .FromJust();

        Local<String> initial_runtime;
        // TODO: Still really hacky :D
        if (this->external_polyfill_bootstrapper.empty())
        {
            initial_runtime = String::NewFromUtf8(isolate, GetEmbeddedPolyfillData().c_str(), NewStringType::kNormal)
                                  .ToLocalChecked();
        }
        else
        {
            initial_runtime =
                String::NewFromUtf8(isolate, this->external_polyfill_bootstrapper.c_str(), NewStringType::kNormal)
                    .ToLocalChecked();
        }
        Local<Script> initial_runtime_script = Script::Compile(context, initial_runtime).ToLocalChecked();
        initial_runtime_script->Run(context).ToLocalChecked();

        auto module = Winx::Modules::CheckModule(
            context, Winx::Modules::LoadModule(context, program_file, Winx::Util::read_file(program_file).c_str()));
        Winx::Modules::ExecuteModule(context, module);

        // TODO: Do we need module-less support?
        // Local<String> source_code =
        //     String::NewFromUtf8(isolate, program_file.c_str(),
        //                             NewStringType::kNormal)
        //         .ToLocalChecked();
        // Local<Script> script =
        //     Script::Compile(context, source_code).ToLocalChecked();
        // auto script_result = script->Run(context).ToLocalChecked();
        // String::Utf8Value utf8(isolate, script_result);
        // cout << "Result: " << *utf8 << endl;
    }
}

void WinxEngine::SetupBinding(Local<ObjectTemplate> parent, Local<ObjectTemplate> object, string object_name)
{
    parent->Set(isolate, object_name.c_str(), object);
}

WinxEngine::WinxEngine(string program_file, string program_embedded_request, string external_polyfill_bootstrapper)
{
    this->program_file = program_file;
    this->program_emebdded_request = program_embedded_request;
    this->external_polyfill_bootstrapper = external_polyfill_bootstrapper;

    this->InitializeEngine();
}

string WinxEngine::GetEmebeddedRequest()
{
    return this->program_emebdded_request;
}

void WinxEngine::RunProgram()
{
    Isolate *isolate = this->isolate;
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
    Local<ObjectTemplate> winx = ObjectTemplate::New(isolate);

    SetupBinding(winx, Winx::Bindings::Console::EngineBind(isolate), "console");
    SetupBinding(winx, Winx::Bindings::FileSystem::EngineBind(isolate), "fs");
    SetupBinding(winx, Winx::Bindings::Os::EngineBind(isolate), "os");
    SetupBinding(global, winx, "Winx");

    if (!this->program_emebdded_request.empty())
    {
        global->SetAccessor(String::NewFromUtf8(isolate, "request", NewStringType::kNormal).ToLocalChecked(),
                            EmbeddedRequestGetterAccessor);
    }

    Local<Context> context = Context::New(isolate, NULL, global);
    this->context = context;
    this->ExecuteScript();
}

void WinxEngine::DisposeEngine()
{
    this->isolate->Dispose();
    V8::Dispose();
    V8::DisposePlatform();
    delete this->create_params.array_buffer_allocator;
}

string GetEmbeddedPolyfillData()
{
    char *poly_data = (char *)malloc(sizeof(char) * (polyfills_Winx_js_len + 1));
    for (int i = 0; i < polyfills_Winx_js_len; i++)
    {
        poly_data[i] = polyfills_Winx_js[i];
    }
    poly_data[polyfills_Winx_js_len] = '\0';
    return string(poly_data);
}

void EmbeddedRequestGetterAccessor(Local<String> property, const PropertyCallbackInfo<Value> &info)
{
    Local<External> externalWinxEngine =
        info.This()
            ->Get(info.GetIsolate()->GetCurrentContext(),
                  String::NewFromUtf8(info.GetIsolate(), "winxEngine", NewStringType::kNormal).ToLocalChecked())
            .ToLocalChecked()
            .As<External>();
    WinxEngine *winxEngine = static_cast<WinxEngine *>(externalWinxEngine->Value());

    Isolate *isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<String> request =
        String::NewFromUtf8(isolate, winxEngine->GetEmebeddedRequest().c_str(), NewStringType::kNormal)
            .ToLocalChecked();
    Local<Value> json = JSON::Parse(context, request).ToLocalChecked();

    info.GetReturnValue().Set(json);
}

} // namespace Winx

int internal_main(int argc, char *argv[])
{
    string filename;
    string environment_embedded_request;

    CLI::App app{"Winx - A V8-based JavaScript runtime for Macintosh."};
    app.add_flag("-D,--debug", IS_DEBUG_MODE_ENABLED, "Enable debug mode (default: false)")->default_val(false);
    app.add_option("-r,--request", environment_embedded_request, "A JSON to pass the runtime");
    app.add_option("filename", filename, "The program needed to execute")->required();
    CLI11_PARSE(app, argc, argv);

    string bootstrapper = Winx::Util::read_file(WinxConfig::get_winx_flag("polyfills_file"));

    Winx::WinxEngine engine(filename, environment_embedded_request, bootstrapper);
    engine.RunProgram();
    engine.DisposeEngine();
    return 0;
}

int main(int argc, char *argv[])
{
    internal_main(argc, argv);
}

#endif // WINX_MAIN
