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
#define RTY_ASSERT(x, ...) { if(!(x)) { RTY_BASE_ERROR("Assertion Failed: {0}", __VA_ARGS__); RTY_DEBUG_BREAK(); } }
#define RTY_RAYTRACER_ASSERT(x, ...) { if(!(x)) { RTY_RAYTRACER_ERROR("Assertion Failed: {0}", __VA_ARGS__); RTY_DEBUG_BREAK(); } }
#else
#define RTY_ASSERT(x, ...)
#define RTY_RAYTRACER_ASSERT(x, ...)
#endif

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;