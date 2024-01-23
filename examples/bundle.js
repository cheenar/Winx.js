const IMPORT_STATEMENT_REGEX = /import "(?<import_file>.*?)"/g;
const POLYFILL_DIRECTORY = "polyfills/";
const TEMPLATE_FILE = "Winx.js.template";

function read_file(filename) {
  return Winx.fs.blind_read_file(filename);
}

function safe_read_file(filename) {
  let polyfill_directory_read = read_file(POLYFILL_DIRECTORY + filename);
  return polyfill_directory_read ? polyfill_directory_read : read_file(filename);
}

function bundle() {
  let template = safe_read_file(TEMPLATE_FILE);
  let imports = [...template.matchAll(IMPORT_STATEMENT_REGEX)];
  imports.forEach((import_statement) => {
    let import_file = import_statement.groups.import_file;
    let import_data = safe_read_file(import_file);
    template = template.replace(import_statement[0], import_data);
  });
  console.log(template);
}

bundle();