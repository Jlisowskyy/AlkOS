#ifndef LIBC_INCLUDE_NEW_HPP_
#define LIBC_INCLUDE_NEW_HPP_

#include <stddef.h>

#ifdef __cplusplus
inline void *operator new(size_t, void *ptr) noexcept { return ptr; }

inline void *operator new[](size_t, void *ptr) noexcept { return ptr; }

inline void operator delete(void *, void *) noexcept {}

inline void operator delete[](void *, void *) noexcept {}
#endif  // __cplusplus

#endif  // LIBC_INCLUDE_NEW_HPP_
