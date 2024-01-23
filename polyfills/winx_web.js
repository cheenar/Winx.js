// Console
console.log = Winx.console.log;
console.debug = Winx.console.debug;

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
