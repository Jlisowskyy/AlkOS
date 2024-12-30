#ifndef KERNEL_INCLUDE_DEFINES_HPP_
#define KERNEL_INCLUDE_DEFINES_HPP_

#define PACK __attribute__((__packed__))

#define NO_RET __attribute__((noreturn))

#define FORCE_INLINE inline __attribute__((always_inline))

#define WRAP_CALL static FORCE_INLINE

#define FAST_CALL static FORCE_INLINE

#define STRINGIFY(x) #x
#define TOSTRING(x)  STRINGIFY(x)

#ifdef __USE_DEBUG_OUTPUT__
static constexpr bool kUseDebugOutput = true;
#else
static constexpr bool kUseDebugOutput = false;
#endif  // __USE_DEBUG_OUTPUT__

#ifdef __DEBUG_ENABLE_TRACES__
static constexpr bool kDebugTracesEnabled = true;
#else
static constexpr bool kDebugTracesEnabled = false;
#endif  // __DEBUG_ENABLE_TRACES__

#ifdef __ALKOS_TESTS_BUILD__
static constexpr bool kIsAlkosTestBuild = true;
#else
static constexpr bool kIsAlkosTestBuild = false;
#endif  // __ALKOS_TESTS_BUILD__

#endif  // KERNEL_INCLUDE_DEFINES_HPP_
