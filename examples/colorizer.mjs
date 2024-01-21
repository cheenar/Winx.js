let ASCII_ESCAPE_CODE = "\u001b";
let RESET_CODE = `${ASCII_ESCAPE_CODE}[0m`;

let EIGHT_BIT_RESET = "0";
let EIGHT_BIT_FOREGROUND_DEFAULT = "39";
let EIGHT_BIT_BACKGROUND_DEFAULT = "49";

let COLOR_MODE = {
  normal: "0",
  bold: "1",
  faint: "2",
  italic: "3",
  underline: "4",
  blink: "5",
  blink_fast: "6",
  reverse: "7",
  conceal: "8",
  crossed_out: "9"
}

let EIGHT_BIT_FOREGROUND_COLOR = {
  black: "30",
  red: "31",
  green: "32",
  yellow: "33",
  blue: "34",
  magenta: "35",
  cyan: "36",
  white: "37",
  bright_black: "90",
  bright_red: "91",
  bright_green: "92",
  bright_yellow: "93",
  bright_blue: "94",
  bright_magenta: "95",
  bright_cyan: "96",
  bright_white: "97"
}

let EIGHT_BIT_BACKGROUND_COLOR = {
  black: "40",
  red: "41",
  green: "42",
  yellow: "43",
  blue: "44",
  magenta: "45",
  cyan: "46",
  white: "47",
  bright_black: "100",
  bright_red: "101",
  bright_green: "102",
  bright_yellow: "103",
  bright_blue: "104",
  bright_magenta: "105",
  bright_cyan: "106",
  bright_white: "107"
}

class Colorizer {
  constructor() {
    this.mode = EIGHT_BIT_RESET;
    this.background = EIGHT_BIT_BACKGROUND_DEFAULT;
    this.foreground = EIGHT_BIT_FOREGROUND_DEFAULT;

    Object.entries(EIGHT_BIT_FOREGROUND_COLOR).forEach(([color, code]) => {
      this[color] = () => {
        this.foreground = code;
        return this;
      }
    })

    Object.entries(EIGHT_BIT_BACKGROUND_COLOR).forEach(([color, code]) => {
      this["bg_" + color] = () => {
        this.background = code;
        return this;
      }
    })

    Object.entries(COLOR_MODE).forEach(([mode, code]) => {
      this[mode] = () => {
        this.mode = code;
        return this;
      }
    })
  }

  compile(str) {
    let color_suffix = "";
    if (this.foreground) {
      color_suffix += `;${this.foreground}`;
    }
    if (this.background) {
      color_suffix += `;${this.background}`;
    }
    let result = `${ASCII_ESCAPE_CODE}[${this.mode}${color_suffix}m${str}${RESET_CODE}`

    // Resets everything for the next call
    this.mode = EIGHT_BIT_RESET;
    this.background = EIGHT_BIT_BACKGROUND_DEFAULT;
    this.foreground = EIGHT_BIT_FOREGROUND_DEFAULT;

    return result;
  }

  foreground_rgb(r, g, b) {
    this.mode = `38;2;${r};${g};${b}`;
    this.foreground = "";
    return this;
  }

  background_rgb(r, g, b) {
    this.mode = `48;2;${r};${g};${b}`;
    this.background = "";
    return this;
  }

  foreground_256(color) {
    this.mode = `38;5;${color}`;
    this.foreground = "";
    return this;
  }

  background_256(color) {
    this.mode = `48;5;${color}`;
    this.background = "";
    return this;
  }

  static make() {
    return new Colorizer();
  }
}

export const colorize = Colorizer.make();
export function gradientize(str, offset = 0, blending = 0, is_background = true) {
  let result = "";
  let k = offset;
  for (let i = 0; i < str.length; i++) {
    result += is_background ? colorize.background_256(k % 256).compile(str[i])
      : colorize.foreground_256(k % 256).compile(str[i]);
    if (i % blending == 0) {
      k += 1;
    }
  }
  return result;
}