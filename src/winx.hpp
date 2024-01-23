#ifndef __SRC_WINX_HPP__
#define __SRC_WINX_HPP__

#include <libplatform/libplatform.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <toml.h>
#include <v8.h>

#include "embedded_winx_polyfill.h"
#include "winx_config.hpp"
#include "winx_console.hpp"
#include "winx_fs.hpp"
#include "winx_globals.hpp"
#include "winx_modules.hpp"
#include "winx_os.hpp"
#include "winx_platform.hpp"
#include "winx_util.hpp"
#include <cli11/CLI11.hpp>

// TODO: This is a hacky way to set the global debug flag.
extern bool IS_DEBUG_MODE_ENABLED;

extern unsigned int polyfills_Winx_js_len;
extern unsigned char polyfills_Winx_js[];

using std::string;
using std::unique_ptr;
using v8::Boolean;
using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::ObjectTemplate;
using v8::PropertyCallbackInfo;
using v8::String;
using v8::Value;

namespace Winx
{

/**
 * @brief Returns the contents of the embedded polyfill data from the
 * polyfills_Winx_js string. This data is generated as part of the Makefile
 * via xxd. This is a workaround to the fact that the polyfill data would need
 * to be portable across platforms and the only way to do that is to embed it.
 *
 * @return string Safe polyfill data string
 */
string GetEmbeddedPolyfillData();

/**
 * @brief Produces a getter method for retrieving the embedded request data. Upon Engine startup,
 * a JSON string can be embedded directly into the runtime in the form of a variable called "request".
 *
 * This method provides a typed interface to seed the runtime with data. An alternative approach may have
 * been allowing any number of JSON files and names to be loaded into the Runtime via the .toml, or inline
 * parsed arguments.
 *
 * TODO: Investigate multiple context configuration.
 *
 * @param property The property being retrieved.
 * @param info Property callback information to retrieve the engine instance and return the data.
 */
void EmbeddedRequestGetterAccessor(Local<String> property, const PropertyCallbackInfo<Value> &info);

/**
 * @brief WinxEngine is the main class for the Winx runtime. It is responsible for creation and
 * management of the V8 runtime.
 */
class WinxEngine
{
  private:
    // The program file name to be executed.
    string program_file;

    // The embedded request data.
    string program_emebdded_request;

    // The external polyfill bootstrapper. This is the polyfill data that is loaded before script execution.
    string external_polyfill_bootstrapper;

    // The V8 Isolate instance.
    Isolate *isolate;

    // The initialize V8 isolate creation parameters.
    Isolate::CreateParams create_params;

    // The V8 platform instance.
    unique_ptr<Winx::WinxPlatform> platform;

    // The V8 context instance.
    Local<Context> context;

    /**
     * @brief Initializes the V8 engine. Sets up the ICU data and the V8 platform. Masses the
     * dynamic module resolution callback.
     */
    void InitializeEngine();

    /**
     * @brief Executes the script file provided in the ctr. Creates a scope context, executes the polyfill
     * source, and then executes the program file in module mode.
     */
    void ExecuteScript();

    /**
     * @brief Provides a mechanism to bind a V8 object to the global scope. This is used to seed
     * the runtime with native functions.
     *
     * @param parent The globalThis parameter.
     * @param object The object to bind.
     * @param object_name The name of the object to bind. This is what will appear in JS land under the Winx object.
     */
    void SetupBinding(Local<ObjectTemplate> parent, Local<ObjectTemplate> object, string object_name);

  public:
    WinxEngine(string program_file, string program_embedded_request, string external_polyfill_bootstrapper);

    /**
     * @brief EmbeddedRequest Getter.
     * @return string The embedded request data.
     */
    string GetEmebeddedRequest();

    /**
     * @brief Runs the engine given the inputs.
     *
     * TODO: Support loading the runtime with functionality via the public methods; enables the ability to
     * embed externally.
     */
    void RunProgram();

    /**
     * @brief Disposes of the engine.
     */
    void DisposeEngine();
};

}; // namespace Winx

#endif // __SRC_WINX_HPP__
