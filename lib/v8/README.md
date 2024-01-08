1. Download [V8](https://v8.dev/docs/build)
2. Build [V8](https://v8.dev/docs/build-gn)

```
# Build arguments go here.
# See "gn args <out_dir> --list" for available build arguments.
clang_use_chrome_plugins=false
is_component_build=false
is_debug=false
strip_debug_info=true
symbol_level=0
target_cpu="arm64"
treat_warnings_as_errors=false
use_custom_libcxx=false
v8_enable_gdbjit=false
v8_enable_i18n_support=false
v8_enable_pointer_compression=false
v8_enable_test_features=false
v8_monolithic=true
v8_use_external_startup_data=false
```
