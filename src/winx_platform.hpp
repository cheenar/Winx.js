#include <v8.h>

namespace Winx
{
/*
---------------------------------------------
WinxPlatform
---------------------------------------------

In the future, we will need to implement a version of v8::Platform that
interpolates with libuv. For now, we can just use the default platform.

TODO: Need to implement an override on the CurrentTimeMilliseconds()/HighResTimeNanoseconds()
to avoid exposing a high-resolution clock for Spectre/Meltdown attacks.
*/
typedef v8::Platform WinxPlatform;

}; // namespace Winx