/*
--------------------------------------------------------
    Winx.js

This file defines polyfills to enable Web & Node APIs
from the Winx.js standard.
--------------------------------------------------------
*/

// Console
console.log = Winx.console.log;
console.debug = Winx.console.debug;

const formatted = (number, sig_digits = 2) => {
    let factor = Math.pow(10, sig_digits);
    return Math.trunc(number * factor) / factor;
}

// Basic math extension
Winx.math = {}
Winx.math.round = formatted


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