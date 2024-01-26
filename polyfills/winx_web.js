// Console
console.log = function (str = "", newline_char = "\n") {
  Winx.console.log(str, newline_char);
}
console.debug = function (str = "", newline_char = "\n") {
  Winx.console.debug(str, newline_char);
}

const formatted = (number, sig_digits = 2) => {
  let factor = Math.pow(10, sig_digits);
  return Math.trunc(number * factor) / factor;
}

// Basic math extension
Winx.math = {}
Object.defineProperties(Winx.math, {
  round: {
    value: formatted,
    writable: false
  }
})
