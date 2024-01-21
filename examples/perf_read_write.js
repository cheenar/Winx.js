const fs = require("node:fs");

let file = "test2.dat"
console.log(fs.readFileSync(file, 'utf-8'))
