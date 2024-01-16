#include "Winx.h"
#include <../src/winx.cpp>
#include <iostream>
#include <string>

JNIEXPORT void JNICALL Java_Winx_startupV8(JNIEnv* env,
                                           jobject obj,
                                           jstring str) {
  char* argv[2];
  argv[0] = "winx";
  argv[1] = (char*)(*env).GetStringUTFChars(str, 0);
  internal_main(2, argv);
}
