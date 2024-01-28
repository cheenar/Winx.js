import { colorize } from "./colorizer.mjs"

const os = require("node:os")

const formatted = (number, sig_digits = 2) => {
  let factor = Math.pow(10, sig_digits);
  return Math.trunc(number * factor) / factor;
}

const emit = (prefix, message) => {
  console.log(colorize.red().compile(prefix) + ": " + message)
}

console.log("--- CPU ---")

let cpus = Winx.os.cpus()
emit("CPU Count", cpus.length)
emit("CPU Model", cpus[0].model)
emit("CPU Speed", cpus[0].speed / 1000 + ` ${colorize.green().compile("GHz")}`)


console.log("")
console.log("--- Memory ---")

let freeMemory = formatted(os.freemem() * (1 / 1024) * (1 / 1024) * (1 / 1024)) // in GiB
let totalMemory = formatted(os.totalmem() * (1 / 1024) * (1 / 1024) * (1 / 1024)) // in GiB

emit("FreeMem", `${freeMemory} ${colorize.green().compile("GiB")}`)
emit("TotalMem", `${totalMemory} ${colorize.green().compile("GiB")}`)
emit("Ratio", `${formatted(freeMemory / totalMemory) * 100}${colorize.green().compile("%")}`)