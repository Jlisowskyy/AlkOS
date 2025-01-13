#ifndef LIBC_INCLUDE_DEFINES_HPP_
#define LIBC_INCLUDE_DEFINES_HPP_

#if !defined(__ALKOS_KERNEL__) && !__STDC_HOSTED__
#error "Missing environment definition"
#endif

// ------------------------------
// C++ defines
// ------------------------------

#ifdef __cplusplus

#ifdef __ALKOS_KERNEL__
static constexpr bool kIsKernel = true;
#else
static constexpr bool kIsKernel = false;
#endif

#endif // __cplusplus

// ------------------------------
// Lang independent defines
// ------------------------------

#define INLINE    inline __attribute__((always_inline))
#define API_CALL  INLINE
#define WRAP_CALL INLINE
#define FAST_CALL INLINE

// ------------------------------
// Lang specific defines
// ------------------------------

/* C decl */
#ifdef __cplusplus
#define BEGIN_DECL_C extern "C" {
#define END_DECL_C } // extern "C"
#else // __cplusplus
#define BEGIN_DECL_C
#define END_DECL_C
#endif // __cplusplus

/* constexpr */
#ifdef __cplusplus
#define CONSTEPXR static constexpr
#else
#define CONSTEXPR static inline
#endif // __cplusplus

#endif  // LIBC_INCLUDE_DEFINES_HPP_
