#ifndef LIBC_INCLUDE_DEFINES_HPP_
#define LIBC_INCLUDE_DEFINES_HPP_

#if !defined(__ALKOS_KERNEL__) && !__STDC_HOSTED__
#error "Missing environment definition"
#endif

#define INLINE inline __attribute__((always_inline))
#define API_CALL INLINE

#endif // LIBC_INCLUDE_DEFINES_HPP_
