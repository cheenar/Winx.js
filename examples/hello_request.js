import { colorize } from "./colorizer.mjs"

function dump_object(obj, indent = "") {
  for (const [key, value] of Object.entries(obj)) {
    console.log(`${indent}${colorize.red().compile("Key")}: ${key}`, " ")
    if (typeof value === "object") {
      console.log();
      dump_object(value, indent + " ")
    } else {
      console.log(indent + colorize.green().compile("Value") + ": " + value);
    }
  }
}


dump_object(request);

