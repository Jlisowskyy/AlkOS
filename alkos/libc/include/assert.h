#ifndef LIBC_INCLUDE_ASSERT_H_
#define LIBC_INCLUDE_ASSERT_H_

#include <todo.h>

#ifdef __ALKOS_KERNEL__

#include <kernel_assert.hpp>

#define assert(expr) ASSERT(expr)

#else

#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
#define assert(expr) TODO_USERSPACE
#endif // NDEBUG

#endif

#endif // LIBC_INCLUDE_ASSERT_H_
