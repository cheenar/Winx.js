<h1 align="center">
  <img src="./docs/images/winx_logo.png" alt="Winx IA" width="200">
  <br/>
  🦋 Winx.js
  <br />
</h1>

![demo](./docs/images/demo.png)

* Lightweight, JavaScript Runtime
* Built on battle-tested (presumably!) V8 JavaScript Engine
* Building towards JNI support

## Configuration

You can define a `Winx.toml` file in the same directory as executing the `winx` binary. You are able 
to configure any [V8 Flag](./V8_FLAGS.md) by default. 

```toml
[v8]
max-heap-size = 2048
jitless = true
gc-experiment-less-compaction = true
expose-wasm = false

[winx]
polyfills_file = "./polyfills/Winx.js"
modules_folder = "./examples/"
```

### [winx] Config

* `polyfills_file` - Location to load the "polyfills" to support features such as console.log.  

## API

All Winx API methods are available under the `Winx` global object. 

### Filesystem

The `Winx.fs` object provides bindings for accessing the file system. There are two ways to use the filesystem, a "blind" read or write, in which the totality of the file content is handled, or via the `FileHandle` system. 

```js
Winx.fs.blind_file_write("my_file.txt", "Hello Winx!");
Winx.console.log(Winx.fs.blind_file_read("my_file.txt")); // Hello Winx!
```

With the `FileHandle` API, you are able to open files in different modes. Currently, single access modes are allowed i.e. Append(`a`), Write(`w`), or Read(`r`). 

```js
let file_handle = Winx.fs.open("my_file.txt", "w");
file_handle.write("Hello Winx!");
file_handle.close();

Winx.console.log(Winx.fs.blind_file_read("my_file.txt")); // Hello Winx!
```

### OS

The `Winx.os` object provides bindings for operating system related processes. Primarily, this is exposing test functions from libuv. 

```js
Winx.console.log(Winx.os.get_free_memory()) // 16 Gb
Winx.console.log(Winx.os.get_total_memory()) // 64 Gb

Winx.console.log("What's your name?: ")
let input = Winx.os.stdin();
Winx.console.log(`Hello ${input}`)
```

### Console

The `Winx.console` object provides bindings for standard console output functions. 

```js
Winx.console.log("Hello World!");
Winx.console.debug("Hello World!");
```

## Node & Web API Compatibility

In [Winx.js](./src/polyfills/Winx.js), there are some "polyfills" that provide Node and Web-like APIs for cross-runtime compatiblity. Initially, focus is spent on getting Winx's capabilities up to par with similar runtimes, and compatibility will follow. For performance testing, some examples are duplicated and implemented in Node.js or Bun.sh compatible APIs. 