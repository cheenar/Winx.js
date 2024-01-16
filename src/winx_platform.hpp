#include <v8.h>

namespace Winx {
/*
---------------------------------------------
WinxPlatform
---------------------------------------------

In the future, we will need to implement a version of v8::Platform that
interpolates with libuv. For now, we can just use the default platform.
*/
typedef v8::Platform WinxPlatform;
};  // namespace Winx