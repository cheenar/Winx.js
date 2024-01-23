/*
--------------------------------------------------------
    Node.js Polyfills

These polyfills enable Node APIs for performance testing
--------------------------------------------------------
*/
Winx.node = {}
Object.defineProperties(Winx.node, {
  fs: {
    value: {
      readFileSync: Winx.fs.blind_read_file,
      writeFileSync: Winx.fs.blind_write_file
    },
    writable: false
  },
  os: {
    value: {
      totalmem: Winx.os.get_total_memory,
      freemem: Winx.os.get_free_memory
    },
    writable: false
  }
});

Winx.node.modules = {
  "fs": Winx.node.fs,
  "os": Winx.node.os
}

function require(module) {
  nodeless_module = module.replace("node:", "");
  return Winx.node.modules[nodeless_module];
}