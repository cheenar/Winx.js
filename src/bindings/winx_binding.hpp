#ifndef __WINX_BINDING_HPP__
#define __WINX_BINDING_HPP__

#include <v8.h>
#include <string>

using namespace v8;

inline Local<ObjectTemplate> create_winx_object_binding(Isolate* isolate) {
  Local<ObjectTemplate> object = ObjectTemplate::New(isolate);
  return object;
}

inline void create_winx_function_binding(Isolate* isolate,
                                         Local<ObjectTemplate> object,
                                         std::string function_name,
                                         FunctionCallback callback) {
  Local<FunctionTemplate> function = FunctionTemplate::New(isolate, callback);
  object->Set(isolate, function_name.c_str(), function);
}

#endif  // __WINX_BINDING_HPP__