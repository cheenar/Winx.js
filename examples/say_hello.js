import { colorize, gradientize } from "./colorizer.mjs";

let my_name = Winx.os.stdin(
  `${colorize.bold()
    .bg_bright_yellow()
    .black()
    .compile("What is your name?")}: `
)
console.log(gradientize(`Hello, ${my_name}!`, 70, 3, false))