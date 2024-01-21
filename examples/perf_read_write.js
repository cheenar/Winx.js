const fs = require("node:fs");

let file = "test.dat"
console.log(fs.readFileSync(file, 'utf-8'))
