#!/usr/bin/env python3

import re

IMPORT_STATEMENT_REGEX = re.compile(r'import \"(.*?)\"')
POLYFILL_DIRECTORY = "polyfills/"
TEMPLATE_FILE = "Winx.js.template" 
IS_DIFF_MODE = False

def read_file(filename):
  try:
    with open(filename, 'r') as file:
      return file.read().strip()
  except:
    with open(POLYFILL_DIRECTORY + filename, 'r') as file:
      return file.read().strip()

def main():
  template = read_file(POLYFILL_DIRECTORY + TEMPLATE_FILE)
  imports = IMPORT_STATEMENT_REGEX.findall(template)
  for import_statement in imports:
    polyfill = read_file(import_statement)
    template = template.replace("import \"" + import_statement + "\"", polyfill)
  if IS_DIFF_MODE:
    print(template)
  else:
    with open("polyfills/Winx.js", "w") as winx:
      winx.write(template)


if __name__ == "__main__":
  main()