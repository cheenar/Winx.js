import { say_hello } from "./examples/lib.mjs"

let my_name = Winx.os.stdin("What is your name?: ")
Winx.console.log(say_hello(my_name));