#include "Winx.h"
#include <../src/winx.cpp>
#include <iostream>
#include <string>

static void invoke_jni_helloJava(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    JNIEnv *env = CONTEXT_GLOBAL_STORE_RETRIEVE(info.GetIsolate(), JNIEnv *, "env");
    jobject obj = CONTEXT_GLOBAL_STORE_RETRIEVE(info.GetIsolate(), jobject, "obj");

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
        Isolate *isolate = engine.GetIsolate();
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        engine.SetupBinding(engine.GetGlobalThis(), EngineBind(engine.GetIsolate()), "jni");

        engine.InitializeContextWithGlobalObject();
        Local<Context> context = Local<Context>::New(engine.GetIsolate(), engine.GetContext());
        CONTEXT_GLOBAL_STORE_INFERENCE(isolate, context, env);
        CONTEXT_GLOBAL_STORE_INFERENCE(isolate, context, obj);
    }

    {
        Isolate *isolate = engine.GetIsolate();
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        Local<Context> context = engine.GetContext();
        Context::Scope context_scope(context);
        engine.ExecuteEmbeddedProgram();
    }

    engine.ShutdownEngine();
}
