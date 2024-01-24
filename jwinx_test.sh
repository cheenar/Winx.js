cd jwinx
g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin Winx.cc -std=c++17 -O3 -Wall -pedantic -I./../include/toml++ -I./../include/v8 -I./../include/uv -I./../include -o Winx.o
g++ -dynamiclib -o libwinx.dylib Winx.o  ../obj//winx_config.o  ../obj//winx_modules.o ../obj/winx_globals.o  ../obj//winx_util.o ./../lib/v8/libv8_monolith.a ../lib/libuv/libuv.a -lc
cd ..
java -cp jwinx -Djava.library.path=jwinx/ Winx examples/hello_jni.js
