/*
--------------------------------------------------------
    Node.js Polyfills

These polyfills enable Node APIs for performance testing
--------------------------------------------------------
*/
os = {}
os.totalmem = Winx.os.get_total_memory
os.freemem = Winx.os.get_free_memory

Node = {}
Node.fs = {}
Node.fs.readFileSync = Winx.fs.blind_read_file
Node.fs.writeFileSync = Winx.fs.blind_write_file

function require(module) {
  if (module === "node:fs") {
    return Node.fs;
  }
}