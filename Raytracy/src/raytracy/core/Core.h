#pragma once

#include <memory>
// Platform detection using predefined macros
#ifdef _WIN32
/* Windows x64/x86 */
#ifdef _WIN64
/* Windows x64  */
#define RTY_PLATFORM_WINDOWS
#else
/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#endif

using std::make_shared;
using std::shared_ptr;
using std::sqrt;