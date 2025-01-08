#ifndef KERNEL_INCLUDE_DEFINES_HPP_
#define KERNEL_INCLUDE_DEFINES_HPP_

// ------------------------------
// Attribute macros
// ------------------------------

/* Prevent the compiler from adding padding to structures */
#define PACK __attribute__((__packed__))

/* Indicate that the function will never return */
#define NO_RET __attribute__((noreturn))

/* Force the compiler to always inline the function */
#define FORCE_INLINE_F inline __attribute__((always_inline))

/* Force the compiler to always inline the lambda */
#define FORCE_INLINE_L __attribute__((always_inline))

/* Declare a function as a static inline wrapper */
#define WRAP_CALL static FORCE_INLINE_F

/* Require a function to be inlined for performance reasons */
#define FAST_CALL static FORCE_INLINE_F

// ------------------------------------
// Macro to constexpr conversions
// ------------------------------------

#ifdef __USE_DEBUG_OUTPUT__
static constexpr bool kUseDebugOutput = true;
#else
static constexpr bool kUseDebugOutput = false;
#endif  // __USE_DEBUG_OUTPUT__

#ifdef __USE_DEBUG_TRACES__
static constexpr bool kUseDebugTraces = true;
#else
static constexpr bool kUseDebugTraces = false;
#endif  // __USE_DEBUG_TRACES__

#ifdef __ALKOS_TESTS_BUILD__
static constexpr bool kIsAlkosTestBuild = true;
#else
static constexpr bool kIsAlkosTestBuild = false;
#endif  // __ALKOS_TESTS_BUILD__

#ifdef NDEBUG
static constexpr bool kIsDebugBuild = false;
#else
static constexpr bool kIsDebugBuild = true;
#endif  // NDEBUG

// ------------------------------
// Useful macros
// ------------------------------

/* Convert a token into a string */
#define STRINGIFY(x) #x
/* Apply STRINGIFY to expand macros before conversion */
#define TOSTRING(x)  STRINGIFY(x)

#endif  // KERNEL_INCLUDE_DEFINES_HPP_
