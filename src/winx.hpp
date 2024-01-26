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

/*
 * The polyfill file contents are loaded here. The polyfill data embeds the runtime with
 * JavaScript code, and enables developing the runtime outside of native code.
 */
#define POLYFILL_FILE_CONTENTS polyfills_Winx_js
#define POLYFILL_FILE_LENGTH polyfills_Winx_js_len

#define WINX_EXTERN __attribute__((visibility("default")))
#define WINX_EXTERN_HIDDEN __attribute__((visibility("hidden")))
#define WINX_EXTERN_PROTECTED __attribute__((visibility("protected")))

extern unsigned int polyfills_Winx_js_len;
extern unsigned char polyfills_Winx_js[];

using std::string;
using std::unique_ptr;
using v8::Boolean;
using v8::Context;
using v8::CopyablePersistentTraits;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Global;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::ObjectTemplate;
using v8::Persistent;
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
WINX_EXTERN_HIDDEN string GetEmbeddedPolyfillData();

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
WINX_EXTERN_HIDDEN void EmbeddedRequestGetterAccessor(Local<String> property, const PropertyCallbackInfo<Value> &info);

/**
 * @brief NaiveEngine is the main class for the Winx runtime. It is responsible for creation and
 * management of the V8 runtime.
 */
class WINX_EXTERN NaiveEngine
{
  private:
    // The program file name to be executed.
    const string program_file;

    // The embedded request data.
    const string program_embedded_request;

    // The external polyfill bootstrapper. This is the polyfill data that is loaded before script execution.
    const string external_polyfill_bootstrapper;

    // The initialize V8 isolate creation parameters.
    Isolate::CreateParams create_params;

    // The V8 platform instance.
    unique_ptr<Winx::WinxPlatform> platform;

    // The global binding variable for objects and functions
    Global<ObjectTemplate> globalThis;

    // The V8 context instance.
    Global<Context> context;

    // The V8 Isolate instance.
    Isolate *isolate;

    // Enabled once InitializeContextWithGlobalObject() is invoked, signaling that context is ready to be augmented.
    // Used for adding data to global context.
    bool is_context_configured;

    /**
     * @brief Initializes the V8 engine. Sets up the ICU data and the V8 platform. Masses the
     * dynamic module resolution callback.
     */
    void InternalInitializeEngine();

    /**
     * @brief Executes the script file provided in the ctr. Creates a scope context, executes the polyfill
     * source, and then executes the program file in module mode.
     */
    void InternalExecuteJavascript();

  public:
    /**
     * @brief Construct a new Winx Engine object.
     *
     * TODO: Migrate program_embedded_request to a setup method that can be done in the main() if desired.
     *
     * @param program_file The program file path, will be read locally by the engine.
     * @param program_embedded_request A stringified JSON object to be embedded in the runtime
     * @param external_polyfill_bootstrapper External polyfill script to seed runtime
     */
    NaiveEngine(string program_file, string program_embedded_request, string external_polyfill_bootstrapper);

    /**
     * @brief Provides a mechanism to bind a V8 object template to the global scope. This is used to seed
     * the runtime with native functions.
     *
     * @param parent The globalThis parameter.
     * @param object The object to bind.
     * @param object_name The name of the object to bind. This is what will appear in JS land under the Winx object.
     */
    void SetupBinding(Local<ObjectTemplate> parent, Local<ObjectTemplate> object, string object_name);

    /**
     * @brief Initializes the NaiveEngine with standard bindings. Also configures the GlobalThis() for embedding
     * further into the engine.
     *
     */
    void InitializeStandardWinxRuntimeBindings();

    /**
     * @brief Triggers creation of the context. By default, if the context is not created, it will be upon
     * ExecuteEmbeddedProgram()
     */
    void InitializeContextWithGlobalObject();

    /**
     * @brief Runs the engine given the inputs.
     *
     * TODO: Support loading the runtime with functionality via the public methods; enables the ability to
     * embed externally.
     */
    void ExecuteEmbeddedProgram();

    /**
     * @brief Disposes of the engine.
     */
    void ShutdownEngine();

    /** Getters **/

    /**
     * @brief EmbeddedRequest Getter.
     * @return string The embedded request data.
     */
    inline string GetEmebeddedRequest();

    inline Isolate *GetIsolate();

    inline Local<ObjectTemplate> GetGlobalThis();

    inline Local<Context> GetContext();
};

}; // namespace Winx

#endif // __SRC_WINX_HPP__
