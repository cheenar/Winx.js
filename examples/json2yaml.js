// This test is not ready for use.

import { colorize } from "./colorizer.mjs"

// function dump_list(list, indent = "") {
//   for (const [_, value] of list.entries()) {
//     console.log(`${indent}- `, "");
//     if (typeof value === "number" || typeof value === "string" || typeof value === "boolean") {
//       if (typeof value === "string") {
//         console.log(`"${value.replace("\n", "{NEW}")}"`);
//       } else {
//         console.log(value);
//       }
//     } else if (value === undefined) {
//       console.log("null");
//     } else if (Array.isArray(value)) {
//       console.log()
//       dump_list(value, indent + " ");
//     } else if (typeof value === "object") {
//       console.log();
//       dump_object(value, indent + " ");
//     }
//   }
// }

// function dump_object(object, indent = "") {
//   for (const [key, value] of Object.entries(object)) {
//     console.log(`${indent}${key}: `, "");
//     if (typeof value === "number" || typeof value === "string" || typeof value === "boolean") {
//       if (typeof value === "string") {
//         console.log(`"${value.replace("\n", "{NEW}")}"`);
//       } else {
//         console.log(value);
//       }
//     } else if (Array.isArray(value)) {
//       console.log()
//       dump_list(value, indent + " ");
//     } else if (value === undefined) {
//       console.log("null");
//     }
//     else if (typeof value === "object") {
//       console.log();
//       dump_object(value, indent + " ");
//     }
//   }
// }

// console.log("---");
// dump_object(request);


function dump_object(obj, indent = "") {
  for (const [key, value] of Object.entries(obj)) {
    console.log(`${indent}${key}: `, "");
    if (typeof value === "string") {
      if (value.includes("\n")) {
        console.log("|-")
      }
      let temp_indent = indent + " ";
      console.log(`${temp_indent}${value.replace("\n", "\n" + temp_indent)}`);
    }
  }
}

console.log("---")
dump_object(request);