let file = "./examples/hello_libuv.js"
let file_handle = Winx.fs.open(file, "r");
console.log(file_handle.read());
file_handle.close()
