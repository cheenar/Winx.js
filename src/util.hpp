// This is uninspiredly copied from NodeJS (ref:
// https://github.com/nodejs/node/blob/09da597535583c139adf18d51af42360cbeae42f/src/util.h#L177)
// Intention is to provide helper macros for asserting requirements

#ifndef __SRC_UTIL_HPP__
#define __SRC_UTIL_HPP__

#include <iostream>

#define WINX_EXTERN __attribute__((visibility("default")))
#define WINX_EXTERN_HIDDEN __attribute__((visibility("hidden")))
#define WINX_EXTERN_PROTECTED __attribute__((visibility("protected")))

#define CHECK(expr)                                                                                                    \
    if (!(expr))                                                                                                       \
    {                                                                                                                  \
        std::cerr << "Check failed: " << #expr << std::endl;                                                           \
        abort();                                                                                                       \
    }

#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_NE(a, b) CHECK((a) != (b))
#define CHECK_LT(a, b) CHECK((a) < (b))
#define CHECK_LE(a, b) CHECK((a) <= (b))
#define CHECK_GT(a, b) CHECK((a) > (b))
#define CHECK_GE(a, b) CHECK((a) >= (b))
#define CHECK_NOT_NULL(expr) CHECK((expr) != nullptr)

#define GEN_STRING_ONE_BYTE(isolate, str)                                                                              \
    v8::String::NewFromOneByte(isolate, str, v8::NewStringType::kNormal).ToLocalChecked()

#define GEN_STRING_UTF8(isolate, str) v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal).ToLocalChecked()

#endif // __SRC_UTIL_HPP__