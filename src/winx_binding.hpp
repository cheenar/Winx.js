#ifndef __SRC_WINX_BINDING_HPP__
#define __SRC_WINX_BINDING_HPP__

#include <string>
#include <v8.h>

using v8::FunctionCallback;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::ObjectTemplate;

namespace Winx::Binding
{

/**
 * @brief Creates a winx object binding object. This is an alias for calling ObjectTemplate::New.
 *
 * @param isolate V8 isolate.
 * @return Local<ObjectTemplate> A generated object template.
 */
inline Local<ObjectTemplate> create_winx_object_binding(Isolate *isolate);

/**
 * @brief Create a winx function binding object. Binds the given object with the function callback
 * and the named function.
 *
 * @param isolate V8 isolate.
 * @param object V8 object to bind the function to
 * @param function_name The function name to be used in javascript land.
 * @param callback The function callback.
 */
inline void create_winx_function_binding(Isolate *isolate, Local<ObjectTemplate> object, std::string function_name,
                                         FunctionCallback callback);

} // namespace Winx::Binding

inline Local<ObjectTemplate> Winx::Binding::create_winx_object_binding(Isolate *isolate)
{
    Local<ObjectTemplate> object = ObjectTemplate::New(isolate);
    return object;
}

inline void Winx::Binding::create_winx_function_binding(Isolate *isolate, Local<ObjectTemplate> object,
                                                        std::string function_name, FunctionCallback callback)
{
    Local<FunctionTemplate> function = FunctionTemplate::New(isolate, callback);
    object->Set(isolate, function_name.c_str(), function);
}

#endif // __WINX_BINDING_HPP__