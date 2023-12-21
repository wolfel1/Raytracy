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

#ifdef RTY_DEBUG
#ifdef RTY_PLATFORM_WINDOWS
#define RTY_DEBUG_BREAK() __debugbreak()
#elif defined(RTY_PLATFORM_LINUX)
#include <signal.h>
#define RTY_DEBUG_BREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak!"
#endif
#define RTY_ENABLE_ASSERTS
#else
#define RTY_DEBUG_BREAK()
#endif

#ifdef RTY_ENABLE_ASSERTS
#define RTY_ASSERT(x, ...) { if(!(x)) { RTY_BASE_ERROR("Assertion Failed: {0}", std::format(__VA_ARGS__)); RTY_DEBUG_BREAK(); } }
#else
#define RTY_ASSERT(x, ...)
#endif

#define RTY_BIND_EVENT_FN(fn) [this]<class T>(T& e) { return fn(e); }

using std::make_shared;
using std::shared_ptr;
using std::make_unique;
using std::unique_ptr;


const float infinity = std::numeric_limits<float>::infinity();