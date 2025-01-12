#ifndef LIBC_INCLUDE_DEFINES_HPP_
#define LIBC_INCLUDE_DEFINES_HPP_

#if !defined(__ALKOS_KERNEL__) && !__STDC_HOSTED__
#error "Missing environment definition"
#endif

#ifdef __ALKOS_KERNEL__
static constexpr bool kIsKernel = true;
#else
static constexpr bool kIsKernel = false;
#endif

#define INLINE    inline __attribute__((always_inline))
#define API_CALL  INLINE
#define WRAP_CALL INLINE
#define FAST_CALL INLINE

#endif  // LIBC_INCLUDE_DEFINES_HPP_
