import { colorize } from "./colorizer.mjs"

function dump_object(obj, indent = "") {
  for (const [key, value] of Object.entries(obj)) {
    console.log(indent + colorize.red().compile("Key") + ": " + key);
    if (typeof value === "object") {
      console.log(indent + colorize.green().compile("Value") + ": " + dump_object(value, indent + "  "));
    } else {
      console.log(indent + colorize.green().compile("Value") + ": " + value);
    }
  }
}

let request = '{"glossary":{"title":"example glossary","GlossDiv":{"title":"S","GlossList":{"GlossEntry":{"ID":"SGML","SortAs":"SGML","GlossTerm":"Standard Generalized Markup Language","Acronym":"SGML","Abbrev":"ISO 8879:1986","GlossDef":{"para":"A meta-markup language, used to create markup languages such as DocBook.","GlossSeeAlso":["GML","XML"]},"GlossSee":"markup"}}}}}'
dump_object(JSON.parse(request));

