#ifndef __SRC_WINX_FS__HPP__
#define __SRC_WINX_FS__HPP__

#include <v8.h>

#include "winx_binding.hpp"
#include "winx_util.hpp"

using namespace v8;
using Winx::Util::WinxFileHandle;

namespace Winx::Bindings::FileSystem
{

void read_file(const FunctionCallbackInfo<Value> &args);

void write_file(const FunctionCallbackInfo<Value> &args);

void delete_file(const FunctionCallbackInfo<Value> &args);

Local<ObjectTemplate> winx_file_handle_template(Isolate *isolate);

Local<Object> wrap_file_handle(Isolate *isolate, WinxFileHandle *file_handle);

WinxFileHandle *unwrap_file_handle(Local<Object> obj);

void winx_file_handle_open(const FunctionCallbackInfo<Value> &args);

void winx_file_handle_read(const FunctionCallbackInfo<Value> &args);

void winx_file_handle_write(const FunctionCallbackInfo<Value> &args);

void winx_file_handle_close(const FunctionCallbackInfo<Value> &args);

Local<ObjectTemplate> EngineBind(Isolate *isolate);

} // namespace Winx::Bindings::FileSystem

void Winx::Bindings::FileSystem::read_file(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    if (args.Length() < 1)
    {
        return;
    }
    String::Utf8Value filePath(isolate, args[0]);
    auto fileData = Winx::Util::read_file(std::string(*filePath));
    args.GetReturnValue().Set(
        String::NewFromUtf8(isolate, fileData.c_str(), NewStringType::kNormal, fileData.size()).ToLocalChecked());
}

void Winx::Bindings::FileSystem::write_file(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    if (args.Length() < 2)
    {
        return;
    }
    String::Utf8Value file_path(isolate, args[0]);
    String::Utf8Value file_data(isolate, args[1]);
    Winx::Util::write_file(*file_path, *file_data);
}

void Winx::Bindings::FileSystem::delete_file(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    if (args.Length() < 1)
    {
        return;
    }
    String::Utf8Value file_path(isolate, args[0]);
    std::remove(*file_path);
}

Local<ObjectTemplate> Winx::Bindings::FileSystem::winx_file_handle_template(Isolate *isolate)
{
    EscapableHandleScope handle_scope(isolate);

    Local<ObjectTemplate> file_handle_template = ObjectTemplate::New(isolate);
    file_handle_template->SetInternalFieldCount(1);

    // setup function templates for read/write file
    file_handle_template->Set(String::NewFromUtf8(isolate, "read", NewStringType::kNormal).ToLocalChecked(),
                              FunctionTemplate::New(isolate, winx_file_handle_read));
    file_handle_template->Set(String::NewFromUtf8(isolate, "write", NewStringType::kNormal).ToLocalChecked(),
                              FunctionTemplate::New(isolate, winx_file_handle_write));
    file_handle_template->Set(String::NewFromUtf8(isolate, "close", NewStringType::kNormal).ToLocalChecked(),
                              FunctionTemplate::New(isolate, winx_file_handle_close));

    return handle_scope.Escape(file_handle_template);
}

Local<Object> Winx::Bindings::FileSystem::wrap_file_handle(Isolate *isolate, WinxFileHandle *file_handle)
{
    EscapableHandleScope handle_scope(isolate);

    Local<ObjectTemplate> file_handle_template = winx_file_handle_template(isolate);
    Local<Object> result = file_handle_template->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    Local<External> file_handle_ptr = External::New(isolate, file_handle);
    result->SetInternalField(0, file_handle_ptr);
    return handle_scope.Escape(result);
}

WinxFileHandle *Winx::Bindings::FileSystem::unwrap_file_handle(Local<Object> obj)
{
    Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
    void *ptr = field->Value();
    return static_cast<WinxFileHandle *>(ptr);
}

void Winx::Bindings::FileSystem::winx_file_handle_open(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    String::Utf8Value file_name(isolate, args[0]);
    String::Utf8Value mode(isolate, args[1]);
    WinxFileHandle *file_handle = Winx::Util::open_file(*file_name, *mode);
    args.GetReturnValue().Set(wrap_file_handle(isolate, file_handle));
}

void Winx::Bindings::FileSystem::winx_file_handle_read(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    WinxFileHandle file_handle = *unwrap_file_handle(args.Holder());
    std::string file_data = Winx::Util::read_file(file_handle);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, file_data.c_str(), NewStringType::kNormal).ToLocalChecked());
}

void Winx::Bindings::FileSystem::winx_file_handle_write(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    WinxFileHandle file_handle = *unwrap_file_handle(args.Holder());
    String::Utf8Value file_data(isolate, args[0]);
    Winx::Util::write_file(file_handle, *file_data);
}

void Winx::Bindings::FileSystem::winx_file_handle_close(const FunctionCallbackInfo<Value> &args)
{
    WinxFileHandle *file_handle = unwrap_file_handle(args.Holder());
    Winx::Util::close_file(file_handle);
}

Local<ObjectTemplate> Winx::Bindings::FileSystem::EngineBind(Isolate *isolate)
{
    Local<ObjectTemplate> fs = Winx::Binding::create_winx_object_binding(isolate);
    Winx::Binding::create_winx_function_binding(isolate, fs, "blind_read_file", read_file);
    Winx::Binding::create_winx_function_binding(isolate, fs, "blind_write_file", write_file);
    Winx::Binding::create_winx_function_binding(isolate, fs, "delete", delete_file);
    Winx::Binding::create_winx_function_binding(isolate, fs, "open", winx_file_handle_open);
    return fs;
}

#endif // __SRC_WINX_FS__HPP__