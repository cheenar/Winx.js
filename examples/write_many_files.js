let file_a = Winx.fs.open("a.txt", "w");
let file_b = Winx.fs.open("b.txt", "w");

for (let i = 0; i < 100; i++) {
  file_a.write("a");
  file_b.write("b");
}

file_a.close();
file_b.close();

Winx.console.log(`a.txt: ${Winx.fs.blind_read_file("a.txt")}`);
Winx.console.log(`b.txt: ${Winx.fs.blind_read_file("b.txt")}`);

Winx.fs.delete("a.txt");
Winx.fs.delete("b.txt");