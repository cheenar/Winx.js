#include "winx.hpp"

void Winx::NaiveEngine::InternalExecuteJavascript()
{
    Context::Scope context_scope(this->GetContext());
    {
        auto externalWinxEngine = External::New(isolate, this);
        this->GetContext()
            ->Global()
            ->Set(this->GetContext(), String::NewFromUtf8Literal(isolate, "winxEngine"), externalWinxEngine)
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
        Local<Script> initial_runtime_script = Script::Compile(this->GetContext(), initial_runtime).ToLocalChecked();
        initial_runtime_script->Run(this->GetContext()).ToLocalChecked();

        auto module = Winx::Modules::CheckModule(
            this->GetContext(),
            Winx::Modules::LoadModule(this->GetContext(), program_file, Winx::Util::read_file(program_file).c_str()));
        Winx::Modules::ExecuteModule(this->GetContext(), module);
    }
    this->globalThis.Reset();
    this->context.Reset();
}

void Winx::NaiveEngine::SetupBinding(Local<ObjectTemplate> parent, Local<ObjectTemplate> object, string object_name)
{
    parent->Set(isolate, object_name.c_str(), object);
}

Winx::NaiveEngine::NaiveEngine(string program_file, string program_embedded_request,
                               string external_polyfill_bootstrapper)
{
    this->program_file = program_file;
    this->program_emebdded_request = program_embedded_request;
    this->external_polyfill_bootstrapper = external_polyfill_bootstrapper;
    this->is_context_configured = false;

    this->InternalInitializeEngine();
}

inline string Winx::NaiveEngine::GetEmebeddedRequest()
{
    return this->program_emebdded_request;
}

void Winx::NaiveEngine::InitializeStandardWinxRuntimeBindings()
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

    this->globalThis = Global<ObjectTemplate>(isolate, global);
}

void Winx::NaiveEngine::InitializeContextWithGlobalObject()
{
    if (this->is_context_configured)
        return;
    this->context = Global<Context>(isolate, Context::New(isolate, NULL, this->globalThis.Get(isolate)));
    this->is_context_configured = true;
}

void Winx::NaiveEngine::ExecuteEmbeddedProgram()
{
    if (!is_context_configured)
    {
        this->InitializeContextWithGlobalObject();
    }

    Isolate *isolate = this->isolate;
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    this->InternalExecuteJavascript();
}

void Winx::NaiveEngine::ShutdownEngine()
{
    this->isolate->DiscardThreadSpecificMetadata();
    this->isolate->Dispose();
    V8::Dispose();
    V8::DisposePlatform();
    delete this->create_params.array_buffer_allocator;
}

string Winx::GetEmbeddedPolyfillData()
{
    char *poly_data = (char *)malloc(sizeof(char) * (POLYFILL_FILE_LENGTH + 1));
    for (int i = 0; i < POLYFILL_FILE_LENGTH; i++)
    {
        poly_data[i] = POLYFILL_FILE_CONTENTS[i];
    }
    poly_data[POLYFILL_FILE_LENGTH] = '\0';
    return string(poly_data);
}

void Winx::EmbeddedRequestGetterAccessor(Local<String> property, const PropertyCallbackInfo<Value> &info)
{
    Local<External> externalWinxEngine =
        info.This()
            ->Get(info.GetIsolate()->GetCurrentContext(),
                  String::NewFromUtf8(info.GetIsolate(), "winxEngine", NewStringType::kNormal).ToLocalChecked())
            .ToLocalChecked()
            .As<External>();
    NaiveEngine *winxEngine = static_cast<NaiveEngine *>(externalWinxEngine->Value());

    Isolate *isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<String> request =
        String::NewFromUtf8(isolate, winxEngine->GetEmebeddedRequest().c_str(), NewStringType::kNormal)
            .ToLocalChecked();
    Local<Value> json = JSON::Parse(context, request).ToLocalChecked();

    info.GetReturnValue().Set(json);
}

inline Local<ObjectTemplate> Winx::NaiveEngine::GetGlobalThis()
{
    return this->globalThis.Get(this->isolate);
}

inline Local<Context> Winx::NaiveEngine::GetContext()
{
    return this->context.Get(this->isolate);
}

inline Isolate *Winx::NaiveEngine::GetIsolate()
{
    return this->isolate;
}

/**
 * @brief Initializes the V8 Engine and creates the engine specific instance of the V8 isolate.
 * Not intended for multi-isolate in engine-land, will need to create larger wrapper (Realm?).
 */
void Winx::NaiveEngine::InternalInitializeEngine()
{
    V8::InitializeICUDefaultLocation(nullptr); // TODO: internationalization support
    V8::InitializeExternalStartupData(nullptr);
    this->platform = platform::NewDefaultPlatform();
    V8::InitializePlatform(this->platform.get());
    V8::SetFlagsFromString(Winx::Config::get_v8_flags().c_str());
    V8::Initialize();

    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate *isolate = Isolate::New(create_params);

    // Configures async-await module resolution
    isolate->SetHostImportModuleDynamicallyCallback(Winx::Modules::ResolveDynamicModuleCallback);

    this->create_params = create_params;
    this->isolate = isolate;
}

/*--------------------*/
/* The Main Functions */
/*--------------------*/

int internal_main(int argc, char *argv[])
{
    string filename;
    string environment_embedded_request;

    CLI::App app{"Winx - A V8-based JavaScript runtime for Macintosh."};
    app.add_flag("-D,--debug", IS_DEBUG_MODE_ENABLED, "Enable debug mode (default: false)")->default_val(false);
    app.add_option("-r,--request", environment_embedded_request, "A JSON to pass the runtime");
    app.add_option("filename", filename, "The program needed to execute")->required();
    CLI11_PARSE(app, argc, argv);

    string bootstrapper = Winx::Util::read_file(Winx::Config::get_winx_flag(WINX_CONFIG_POLYFILLS));

    Winx::NaiveEngine engine(filename, environment_embedded_request, bootstrapper);
    engine.InitializeStandardWinxRuntimeBindings();

    {
        Isolate *isolate = engine.GetIsolate();
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        engine.SetupBinding(engine.GetGlobalThis(), Winx::Bindings::Os::EngineBind(engine.GetIsolate()), "custom_bind");
        engine.InitializeContextWithGlobalObject();
        engine.ExecuteEmbeddedProgram();
    }

    engine.ShutdownEngine();
    return 0;
}

int main(int argc, char *argv[])
{
    internal_main(argc, argv);
}