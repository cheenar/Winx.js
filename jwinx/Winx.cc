#include "Winx.h"
#include <../src/winx.cpp>
#include <iostream>
#include <string>

static void invoke_jni_helloJava(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> global = info.GetIsolate()->GetCurrentContext()->Global();
    Local<External> externalEnv =
        global
            ->Get(info.GetIsolate()->GetCurrentContext(),
                  String::NewFromUtf8(info.GetIsolate(), "JNI_ENV", NewStringType::kNormal).ToLocalChecked())
            .ToLocalChecked()
            .As<External>();
    JNIEnv *env = static_cast<JNIEnv *>(externalEnv->Value());

    Local<External> externalObj =
        global
            ->Get(info.GetIsolate()->GetCurrentContext(),
                  String::NewFromUtf8(info.GetIsolate(), "JNI_OBJ", NewStringType::kNormal).ToLocalChecked())
            .ToLocalChecked()
            .As<External>();
    jobject obj = static_cast<jobject>(externalObj->Value());

    jclass cls = env->GetObjectClass(obj);
    jmethodID methodId = env->GetMethodID(cls, "helloJava", "()V");
    if (methodId == nullptr)
    {
        std::cout << "CANNOT FIND JAVA METHOD" << std::endl;
    }
    env->CallVoidMethod(obj, methodId);

    info.GetReturnValue().Set(String::NewFromUtf8Literal(info.GetIsolate(), "Hello Java"));
}

v8::Local<v8::ObjectTemplate> EngineBind(v8::Isolate *isolate)
{
    v8::Local<v8::ObjectTemplate> jni = Winx::Binding::create_winx_object_binding(isolate);
    Winx::Binding::create_winx_function_binding(isolate, jni, "helloJava", invoke_jni_helloJava);
    return jni;
}

JNIEXPORT void JNICALL Java_Winx_startupV8(JNIEnv *env, jobject obj, jstring str)
{
    // char *argv[2];
    // argv[0] = "winx";
    // argv[1] = (char *)(*env).GetStringUTFChars(str, 0);
    // internal_main(2, argv);

    IS_DEBUG_MODE_ENABLED = false;

    string bootstrapper = Winx::Util::read_file(Winx::Config::get_winx_flag(WINX_CONFIG_POLYFILLS));
    Winx::NaiveEngine engine((char *)(*env).GetStringUTFChars(str, 0), "", bootstrapper);
    engine.InitializeStandardWinxRuntimeBindings();

    {
        Isolate *isolate = engine.isolate;
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        engine.SetupBinding(engine.globalThis.Get(engine.isolate), EngineBind(engine.isolate), "jni");

        engine.InitializeContextWithGlobalObject();
        Local<Context> context = Local<Context>::New(engine.isolate, engine.context);
        context->Global()
            ->Set(context, String::NewFromUtf8(isolate, "JNI_ENV", NewStringType::kNormal).ToLocalChecked(),
                  v8::External::New(isolate, env))
            .FromJust();

        context->Global()
            ->Set(context, String::NewFromUtf8(isolate, "JNI_OBJ", NewStringType::kNormal).ToLocalChecked(),
                  v8::External::New(isolate, obj))
            .FromJust();
    }

    {
        Isolate *isolate = engine.isolate;
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        Local<Context> context = engine.context.Get(engine.isolate);
        Context::Scope context_scope(context);
        engine.ExecuteEmbeddedProgram();
    }

    engine.ShutdownEngine();
}
