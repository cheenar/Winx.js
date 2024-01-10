winx:
	clang++ -o out/winx -I./include/v8 -I./include/uv ./lib/v8/libv8_monolith.a ./lib/libuv/libuv.a -std=c++17 -O3 -Wall -pedantic src/Winx.cpp src/util.cpp