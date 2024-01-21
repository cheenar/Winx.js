#ifndef WINX_FS_HPP
#define WINX_FS_HPP

#include <v8.h>

#include "../winx_util.hpp"
#include "winx_binding.hpp"

using namespace v8;

namespace Winx::Bindings::FileSystem {

// Handle-less Functions

void read_file(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1) {
    return;
  }
  v8::String::Utf8Value filePath(isolate, args[0]);
  auto fileData = Winx::Util::read_file(std::string(*filePath));
  args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, fileData.c_str(),
                                                    v8::NewStringType::kNormal,
                                                    fileData.size())
                                .ToLocalChecked());
}

void write_file(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 2) {
    return;
  }
  v8::String::Utf8Value file_path(isolate, args[0]);
  v8::String::Utf8Value file_data(isolate, args[1]);
  Winx::Util::write_file(*file_path, *file_data);
}

void delete_file(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1) {
    return;
  }
  v8::String::Utf8Value file_path(isolate, args[0]);
  std::remove(*file_path);
}

// Wrapping the WinxFileHandler...

void winx_file_handle_read(const FunctionCallbackInfo<Value>& args);
void winx_file_handle_write(const FunctionCallbackInfo<Value>& args);
void winx_file_handle_close(const FunctionCallbackInfo<Value>& args);

Local<ObjectTemplate> winx_file_handle_template(Isolate* isolate) {
  EscapableHandleScope handle_scope(isolate);

  Local<ObjectTemplate> file_handle_template = ObjectTemplate::New(isolate);
  file_handle_template->SetInternalFieldCount(1);

  // setup function templates for read/write file
  file_handle_template->Set(
      String::NewFromUtf8(isolate, "read", NewStringType::kNormal)
          .ToLocalChecked(),
      FunctionTemplate::New(isolate, winx_file_handle_read));
  file_handle_template->Set(
      String::NewFromUtf8(isolate, "write", NewStringType::kNormal)
          .ToLocalChecked(),
      FunctionTemplate::New(isolate, winx_file_handle_write));
  file_handle_template->Set(
      String::NewFromUtf8(isolate, "close", NewStringType::kNormal)
          .ToLocalChecked(),
      FunctionTemplate::New(isolate, winx_file_handle_close));

  return handle_scope.Escape(file_handle_template);
}

Local<Object> wrap_file_handle(Isolate* isolate,
                               Winx::Util::WinxFileHandle* file_handle) {
  EscapableHandleScope handle_scope(isolate);

  Local<ObjectTemplate> file_handle_template =
      winx_file_handle_template(isolate);
  Local<Object> result =
      file_handle_template->NewInstance(isolate->GetCurrentContext())
          .ToLocalChecked();
  Local<External> file_handle_ptr = External::New(isolate, file_handle);
  result->SetInternalField(0, file_handle_ptr);
  return handle_scope.Escape(result);
}

Winx::Util::WinxFileHandle unwrap_file_handle(Local<Object> obj) {
  Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
  void* ptr = field->Value();
  return *static_cast<Winx::Util::WinxFileHandle*>(ptr);
}

void winx_file_handle_open(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  String::Utf8Value file_name(isolate, args[0]);
  String::Utf8Value mode(isolate, args[1]);
  Winx::Util::WinxFileHandle* file_handle =
      Winx::Util::open_file(*file_name, *mode);
  args.GetReturnValue().Set(wrap_file_handle(isolate, file_handle));
}

void winx_file_handle_read(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Winx::Util::WinxFileHandle file_handle = unwrap_file_handle(args.Holder());
  std::string file_data = Winx::Util::read_file(file_handle);
  args.GetReturnValue().Set(
      String::NewFromUtf8(isolate, file_data.c_str(), NewStringType::kNormal)
          .ToLocalChecked());
}

void winx_file_handle_write(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Winx::Util::WinxFileHandle file_handle = unwrap_file_handle(args.Holder());
  String::Utf8Value file_data(isolate, args[0]);
  Winx::Util::write_file(file_handle, *file_data);
}

void winx_file_handle_close(const FunctionCallbackInfo<Value>& args) {
  Winx::Util::WinxFileHandle file_handle = unwrap_file_handle(args.Holder());
  Winx::Util::close_file(file_handle);
}

// Engine Bindings...

v8::Local<v8::ObjectTemplate> EngineBind(v8::Isolate* isolate) {
  v8::Local<v8::ObjectTemplate> fs = create_winx_object_binding(isolate);
  create_winx_function_binding(isolate, fs, "blind_read_file", read_file);
  create_winx_function_binding(isolate, fs, "blind_write_file", write_file);
  create_winx_function_binding(isolate, fs, "delete", delete_file);
  create_winx_function_binding(isolate, fs, "open", winx_file_handle_open);
  return fs;
}

}  // namespace Winx::Bindings::FileSystem

#endif  // WINX_FS_HPP