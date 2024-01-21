let file_name = "my_file.txt";

let file = Winx.fs.open(file_name, "w");
file.write("Hello Winx File Handles 🦋!");
file.close();

file = Winx.fs.open(file_name, "r");
console.log(file.read());
file.close()

file = Winx.fs.open(file_name, "a");
file.write("\nShowing off append mode");
file.close();

file = Winx.fs.open(file_name, "r");
console.log(file.read());
file.close()

Winx.fs.delete(file_name);