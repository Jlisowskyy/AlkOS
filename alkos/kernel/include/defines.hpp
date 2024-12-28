#ifndef KERNEL_INCLUDE_DEFINES_HPP_
#define KERNEL_INCLUDE_DEFINES_HPP_

#define PACK __attribute__((__packed__))

#define NO_RET __attribute__((noreturn))

#define FORCE_INLINE inline __attribute__((always_inline))

#define WRAP_CALL static FORCE_INLINE

#define STRINGIFY(x) #x
#define TOSTRING(x)  STRINGIFY(x)

#ifdef __DEBUG_TERMINAL_TEST__
static constexpr bool kDebugTerminalEnabled = true;
#else
static constexpr bool kDebugTerminalEnabled = false;
#endif // __DEBUG_TERMINAL_TEST__

#ifdef __DEBUG_ENABLE_TRACES__
static constexpr bool kDebugTracesEnabled = true;
#else
static constexpr bool kDebugTracesEnabled = false;
#endif // __DEBUG_ENABLE_TRACES__

#endif // KERNEL_INCLUDE_DEFINES_HPP_
