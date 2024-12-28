#ifndef KERNEL_INCLUDE_ASSERT_HPP_
#define KERNEL_INCLUDE_ASSERT_HPP_

#include <defines.hpp>
#include <panic.hpp>
#include <todo.hpp>

// ------------------------------
// Int type asserts
// ------------------------------

#define FAIL_KERNEL(expr)                                                                                              \
    KernelPanic("Assertion failed: " TOSTRING(expr) " at file: " __FILE__ " and line: " TOSTRING(__LINE__));

/* c-like debug only assert */
#ifdef NDEBUG

#define ASSERT(expr)

#else

#define ASSERT(expr)                                                                                                   \
    if (!(expr))                                                                                                       \
    {                                                                                                                  \
        FAIL_KERNEL(expr);                                                                                             \
    }

#endif // NDEBUG

/* release assert */
#define ASSERT_ALWAYS(expr)                                                                                            \
    if (!(expr))                                                                                                       \
    {                                                                                                                  \
        FAIL_KERNEL(expr);                                                                                             \
    }

// ------------------------------
// Verbose asserts
// ------------------------------

/**
 * Verbose assertions to compare expected and actual values.
 *
 * @param expected The expected value.
 * @param value The actual value.
 *
 * TODO: Add descriptive prints to these macros when snprintf or equivalent
 * functionality becomes available. This will improve the clarity of assertion
 * failures by providing detailed context.
 *
 * Additionally, consider defining the following macros for greater versatility:
 * - ASSERT_LT(expected, value): Checks if `expected` is less than `value`.
 * - ASSERT_GT(expected, value): Checks if `expected` is greater than `value`.
 * - ASSERT_LE(expected, value): Checks if `expected` is less than or equal to `value`.
 * - ASSERT_GE(expected, value): Checks if `expected` is greater than or equal to `value`.
 */

TODO_BY_THE_END_OF_MILESTONE0

#define ASSERT_EQ(expected, value)                                                                                     \
    if (expected != value)                                                                                             \
    {                                                                                                                  \
        FAIL_KERNEL(expected != value);                                                                                \
    }

#define ASSERT_NEQ(expected, value)                                                                                    \
    if (expected == value)                                                                                             \
    {                                                                                                                  \
        FAIL_KERNEL(expected == value);                                                                                \
    }

#endif // KERNEL_INCLUDE_ASSERT_HPP_
