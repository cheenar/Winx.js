#include "winx_modules.hpp"

MaybeLocal<Module> Winx::Modules::LoadModule(Local<Context> context,
                                             string module_name,
                                             string module_code) {
  Isolate* isolate = context->GetIsolate();
  Local<String> source_code =
      String::NewFromUtf8(isolate, module_code.c_str()).ToLocalChecked();
  Local<String> name =
      String::NewFromUtf8(isolate, module_name.c_str()).ToLocalChecked();
  // This feels hacky because it is hacky. This is mostly the defaults of
  // ScripOrigin but with the is_module set to true
  ScriptOrigin origin(name.As<Value>(), 0, 0, false, -1, Local<Value>(), false,
                      false, true, Local<Data>());
  Context::Scope context_scope(context);
  ScriptCompiler::Source source(source_code, origin);
  MaybeLocal<Module> module = ScriptCompiler::CompileModule(isolate, &source);
  return module;
}

Local<Module> Winx::Modules::CheckModule(Local<Context> context,
                                         MaybeLocal<Module> module) {
  Local<Module> checked_module;
  if (!module.ToLocal(&checked_module)) {
    // Local<Value> exception = isolate->ThrowException(
    //     String::NewFromUtf8(isolate, "Failed to compile module")
    //         .ToLocalChecked());
    // return checked_module;
    exit(1);
  }

  Maybe<bool> result = checked_module->InstantiateModule(
      context, Winx::Modules::ResolveModuleCallback);
  if (result.IsNothing()) {
    // Local<Value> exception = isolate->ThrowException(
    //     String::NewFromUtf8(isolate, "Failed to instantiate module")
    //         .ToLocalChecked());
    // return checked_module;
    exit(1);
  }

  return checked_module;
}

Local<Value> Winx::Modules::ExecuteModule(Local<Context> context,
                                          Local<Module> module,
                                          bool nsObject) {
  Isolate* isolate = context->GetIsolate();

  Local<Value> result;
  if (!module->Evaluate(context).ToLocal(&result)) {
    Local<Value> exception = isolate->ThrowException(
        String::NewFromUtf8(isolate, "Failed to execute module")
            .ToLocalChecked());
    return exception;
  }

  if (nsObject) {
    return module->GetModuleNamespace();
  }

  return result;
}

MaybeLocal<Promise> Winx::Modules::ResolveDynamicModuleCallback(
    Local<Context> context,
    Local<Data> host_defined_options,
    Local<Value> resource_name,
    Local<String> specifier,
    Local<FixedArray> import_assertions) {
  Isolate* isolate = context->GetIsolate();
  Local<Promise::Resolver> resolver =
      Promise::Resolver::New(context).ToLocalChecked();
  MaybeLocal<Promise> promise(resolver->GetPromise());

  String::Utf8Value specifier_string(isolate, specifier);
  Local<Module> mod = Winx::Modules::CheckModule(
      context,
      Winx::Modules::LoadModule(context, *specifier_string,
                                Winx::Util::read_file(*specifier_string)));
  Local<Value> return_value = ExecuteModule(context, mod, true);

  resolver->Resolve(context, return_value).Check();
  return promise;
}

MaybeLocal<Module> Winx::Modules::ResolveModuleCallback(
    Local<Context> context,
    Local<String> specifier,
    Local<FixedArray> import_assertions,
    Local<Module> referrer) {
  Isolate* isolate = context->GetIsolate();
  String::Utf8Value specifier_string(isolate, specifier);
  return LoadModule(context, *specifier_string,
                    Winx::Util::read_file(*specifier_string));
}
