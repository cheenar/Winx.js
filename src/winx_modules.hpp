#ifndef __SRC_WINX_MODULES_HPP__
#define __SRC_WINX_MODULES_HPP__

#include "winx_config.hpp"
#include "winx_util.hpp"
#include <string>
#include <v8.h>

using namespace std;
using namespace v8;

namespace Winx::Modules
{

MaybeLocal<Module> LoadModule(Local<Context> context, string module_name, string module_code);

Local<Module> CheckModule(Local<Context> context, MaybeLocal<Module> module);

Local<Value> ExecuteModule(Local<Context> context, Local<Module> module, bool nsObject = false);

MaybeLocal<Module> ResolveModuleCallback(Local<Context> context, Local<String> specifier,
                                         Local<FixedArray> import_assertions, Local<Module> referrer);

MaybeLocal<Promise> ResolveDynamicModuleCallback(Local<Context> context, Local<Data> host_defined_options,
                                                 Local<Value> resource_name, Local<String> specifier,
                                                 Local<FixedArray> import_assertions);

} // namespace Winx::Modules

#endif // __SRC_WINX_MODULES_HPP__