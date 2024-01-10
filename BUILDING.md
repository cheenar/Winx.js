# Building on Macintosh

```
clang++ -o out/winx -I./include/v8 -I./include/libuv ./lib/v8/libv8_monolith.a ./lib/libuv/libuv.a -std=c++17 -O3 -Wall -pedantic src/Winx.cpp src/util.cpp
```

```
export MallocStackLogging=1
leaks --atExit -- quahog examples/person.js
```