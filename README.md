<div align="center">
  <img src="./docs/images/winx_logo.png" alt="Winx IA" width="200">
  <br/>
  <h3>🦋 Winx.js</h3>
  <br />
  <p>Somewhat a JavaScript Runtime</p>
</div>

![demo](./docs/images/demo.png)

* Lightweight, JavaScript Runtime
* Built on battle-tested (presumably!) V8 JavaScript Engine
* Designed for embedding in Java 21+ Application (FFM)

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
polyfills_file = "./src/polyfills/Winx.js"
```

### [winx] Config

* `polyfills_file` - Location to load the "polyfills" to support features such as console.log.  
