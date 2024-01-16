#ifndef WINX_FS_HPP
#define WINX_FS_HPP

#include <v8.h>

#include "../util.hpp"

namespace Winx::Bindings::FileSystem {

void read_file(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1) {
    return;
  }
  v8::String::Utf8Value filePath(isolate, args[0]);
  auto fileData = WinxUtil::read_file(std::string(*filePath));
  args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, fileData->c_str(),
                                                    v8::NewStringType::kNormal)
                                .ToLocalChecked());
}

}  // namespace Winx::Bindings::FileSystem

#endif  // WINX_FS_HPP