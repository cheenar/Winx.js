// let data = "Hello File!"
// Winx.fs.write_file("my_file.txt", data);

let file_name = "my_file.txt";

let file = Winx.fs.open(file_name, "w");
file.write("Hello Winx File Handles 🦋!");
file.close();

file = Winx.fs.open(file_name, "r");
console.log(file.read());
file.close()

Winx.fs.delete(file_name);



// TODO:
/**
 * let file = Winx.fs.open_file("my_file.txt", "w");
 * file.write("Hello File!");
 * file.close();
 * 
 * let file = Winx.fs.open_file("my_file.txt", "r");
 * let data = file.read();
 * console.log(data);
 * file.close();
 */