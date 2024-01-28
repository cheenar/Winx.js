# Building on Macintosh

> Deprecated. Use the makefile.

```
clang++ -o out/winx -I./include/v8 -I./include/libuv ./lib/v8/libv8_monolith.a ./lib/libuv/libuv.a -std=c++17 -O3 -Wall -pedantic src/Winx.cpp src/util.cpp
```

```
export MallocStackLogging=1
leaks --atExit -- quahog examples/person.js
```

# Completions

I have included some of my VS Code configuration for my own sake. I have tried editing this project in `zed` and found it a bit lacking in comparison to VS Code's feature-set, so I have decided to not include configuration for it at the moment.

To generate the configuration, run the following:

```
make clean
bear -- make
```

You can install bear from homebrew. `brew install bear`
