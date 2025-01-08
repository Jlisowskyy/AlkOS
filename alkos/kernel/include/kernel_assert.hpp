#ifndef KERNEL_INCLUDE_ASSERT_HPP_
#define KERNEL_INCLUDE_ASSERT_HPP_

#include <defines.hpp>
#include <panic.hpp>
#include <string.h>
#include <todo.hpp>
#include <types.hpp>
#include <stdio.h>

// ------------------------------
// Int type asserts
// ------------------------------

#define FAIL_KERNEL(expr)                                                                          \
    KernelPanic(                                                                                   \
        "Assertion failed: " TOSTRING(expr) " at file: " __FILE__ " and line: " TOSTRING(__LINE__) \
    );

/* c-like debug only assert */
#define ASSERT(expr)             \
    if constexpr(kIsDebugBuild){ \
        if (!(expr)) {           \
            FAIL_KERNEL(expr);   \
        }                        \
    }

/* Works also in release build */
#define R_ASSERT(expr)     \
    if (!(expr)) {         \
        FAIL_KERNEL(expr); \
    }

// -------------------------------------
// TODO: temporary print mechanism
// -------------------------------------

/* This print should be replaced by some type driven print mechanism */
TODO_BY_THE_END_OF_MILESTONE1
static constexpr size_t kObjToHexBuffSize = 512;

template<typename ObjT>
void VerboseAssertDumpObjToHex(const ObjT &obj, char *buffer, size_t buffer_size) {
    ASSERT(buffer != nullptr);
    ASSERT(buffer_size > 0);

    const auto obj_bytes = reinterpret_cast<const u8 *>(&obj);
    for (size_t i = 0; i < sizeof(ObjT); ++i) {
        const int bytes_written = snprintf(buffer, buffer_size, "%02X ", obj_bytes[i]);
        ASSERT(bytes_written < static_cast<int>(buffer_size) && "VerboseAssertDumpObjToHex buffer fully used!");

        buffer += bytes_written;
        buffer_size -= bytes_written;
    }
}

/* TODO: temporary overload for char* */
FAST_CALL void VerboseAssertDumpObjToHex(const char *obj, char *buffer, const size_t buffer_size) {
    strncpy(buffer, obj, buffer_size);
}

// ------------------------------
// Base for verbose asserts
// ------------------------------

static constexpr size_t kFailMsgBuffSize = 2048;
static constexpr size_t kFullAssertMsgBuffSize = 512 + kFailMsgBuffSize;

FAST_CALL void VerboseAssertDump(const char *msg, const char *file, const char *line) {
    char full_assert_msg[kFullAssertMsgBuffSize];
    const int bytes_written = snprintf(full_assert_msg, kFullAssertMsgBuffSize,
                                       "Assertion failed at file: %s and line: %s\n%s\n", file, line, msg);
    ASSERT(bytes_written < static_cast<int>(kFullAssertMsgBuffSize) && "VerboseAssertDump buffer fully used!");

    KernelPanic(full_assert_msg);
}

template<class ExpectedT, class ValueT, class CheckerT, class MsgGetterT>
FAST_CALL void VerboseAssertTwoArgBase(const ExpectedT &expected,
                                       const ValueT &value,
                                       CheckerT checker,
                                       MsgGetterT msg_getter,
                                       const char *expected_str,
                                       const char *value_str,
                                       const char *file,
                                       const char *line) {
    if (!checker(expected, value)) {
        char fail_msg[kFailMsgBuffSize];
        char e_obj[kObjToHexBuffSize];
        char v_obj[kObjToHexBuffSize];

        VerboseAssertDumpObjToHex(expected, e_obj, kObjToHexBuffSize);
        VerboseAssertDumpObjToHex(value, v_obj, kObjToHexBuffSize);

        msg_getter(fail_msg, kFailMsgBuffSize, expected_str, value_str, e_obj, v_obj);
        VerboseAssertDump(fail_msg, file, line);
    }
}

template<class ValueT, class CheckerT, class MsgGetterT>
FAST_CALL void VerboseAssertOneArgBase(const ValueT &value,
                                       CheckerT checker,
                                       MsgGetterT msg_getter,
                                       const char *value_str,
                                       const char *file,
                                       const char *line) {
    if (!checker(value)) {
        char fail_msg[kFailMsgBuffSize];
        char v_obj[kObjToHexBuffSize];

        VerboseAssertDumpObjToHex(value, v_obj, kObjToHexBuffSize);

        msg_getter(fail_msg, kFailMsgBuffSize, value_str, v_obj);
        VerboseAssertDump(fail_msg, file, line);
    }
}

// ------------------------------
// EQ Assert base
// ------------------------------

template<class ExpectedT, class ValueT>
FAST_CALL void VerboseAssertEq(const ExpectedT &expected,
                               const ValueT &value,
                               const char *expected_str,
                               const char *value_str,
                               const char *file,
                               const char *line) {
    VerboseAssertTwoArgBase(
        expected, value,
        [](const ExpectedT &e, const ValueT &v) { return e == v; },
        [](char *msg, const int size, const char *e_str, const char *v_str,
           const char *e_dump, const char *v_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (EQ)!\n"
                                               "Actual value does not match the expected value.\n"
                                               "Expected value: %s\n"
                                               "Which is: %s\n"
                                               "Actual value: %s\n"
                                               "Which is: %s\n", e_str, e_dump, v_str, v_dump);
            ASSERT(bytes_written < size && "VerboseAssertEq buffer fully used!");
        },
        expected_str, value_str, file, line
    );
}

// ------------------------------
// NEQ Assert base
// ------------------------------

template<class ExpectedT, class ValueT>
FAST_CALL void VerboseAssertNeq(const ExpectedT &expected,
                                const ValueT &value,
                                const char *expected_str,
                                const char *value_str,
                                const char *file,
                                const char *line) {
    VerboseAssertTwoArgBase(
        expected, value,
        [](const ExpectedT &e, const ValueT &v) { return e != v; },
        [](char *msg, const int size, const char *e_str, const char *v_str,
           const char *e_dump, const char *v_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (NEQ)!\n"
                                               "Actual value does matches the expected value, when it shouldn't.\n"
                                               "Expected value: %s\n"
                                               "Which is: %s\n"
                                               "Actual value: %s\n"
                                               "Which is: %s\n", e_str, e_dump, v_str, v_dump);
            ASSERT(bytes_written < size && "VerboseAssertNeq buffer fully used!");
        },
        expected_str, value_str, file, line
    );
}

// ------------------------------
// TRUE assert base
// ------------------------------

template<class ValueT>
void VerboseAssertTrue(const ValueT &value,
                       const char *value_str,
                       const char *file,
                       const char *line) {
    VerboseAssertOneArgBase(
        value,
        [](const ValueT &v) { return v == true; },
        [](char *msg, const int size, const char *v_str, const char *v_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (TRUE)!\n"
                                               "Given value was supposed to be equal to true!\n"
                                               "Expected value: true\n"
                                               "Actual value: %s\n"
                                               "Which is: %s\n", v_str, v_dump);
            ASSERT(bytes_written < size && "VerboseAssertTrue buffer fully used!");
        },
        value_str, file, line
    );
}

// ------------------------------
// FALSE assert base
// ------------------------------

template<class ValueT>
void VerboseAssertFalse(const ValueT &value,
                        const char *value_str,
                        const char *file,
                        const char *line) {
    VerboseAssertOneArgBase(
        value,
        [](const ValueT &v) { return v == false; },
        [](char *msg, const int size, const char *v_str, const char *v_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (FALSE)!\n"
                                               "Given value was supposed to be equal to false!\n"
                                               "Expected value: True\n"
                                               "Actual value: %s\n"
                                               "Which is: %s\n", v_str, v_dump);
            ASSERT(bytes_written < size && "VerboseAssertFalse buffer fully used!");
        },
        value_str, file, line
    );
}

// ------------------------------
// NOT_NULL assert base
// ------------------------------

template<class ValueT>
void VerboseAssertNotNull(const ValueT &value,
                          const char *value_str,
                          const char *file,
                          const char *line) {
    VerboseAssertOneArgBase(
        value,
        [](const ValueT &v) { return v != nullptr; },
        [](char *msg, const int size, const char *v_str, const char *v_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (NOT_NULL)!\n"
                                               "Given value was supposed to be not null!\n"
                                               "Actual value: %s\n"
                                               "Which is: %s\n", v_str, v_dump);
            ASSERT(bytes_written < size && "VerboseAssertNotNull buffer fully used!");
        },
        value_str, file, line
    );
}

// ------------------------------
// NULL assert base
// ------------------------------

template<class ValueT>
void VerboseAssertNull(const ValueT &value,
                       const char *value_str,
                       const char *file,
                       const char *line) {
    VerboseAssertOneArgBase(
        value,
        [](const ValueT &v) { return v == nullptr; },
        [](char *msg, const int size, const char *v_str, const char *v_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (NULL)!\n"
                                               "Given value was supposed to be null!\n"
                                               "Actual value: %s\n"
                                               "Which is: %s\n", v_str, v_dump);
            ASSERT(bytes_written < size && "VerboseAssertNull buffer fully used!");
        },
        value_str, file, line
    );
}

// ------------------------------
// LT (Less Than) assert base
// ------------------------------

template<class Val1T, class Val2T>
void VerboseAssertLt(const Val1T &val1,
                     const Val2T &val2,
                     const char *val1_str,
                     const char *val2_str,
                     const char *file,
                     const char *line) {
    VerboseAssertTwoArgBase(
        val1, val2,
        [](const Val1T &v1, const Val2T &v2) { return v1 < v2; },
        [](char *msg, const int size, const char *v1_str, const char *v2_str,
           const char *v1_dump, const char *v2_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (LT)!\n"
                                               "First value should be less than second value!\n"
                                               "First value: %s\n"
                                               "Which is: %s\n"
                                               "Second value: %s\n"
                                               "Which is: %s\n", v1_str, v1_dump, v2_str, v2_dump);
            ASSERT(bytes_written < size && "VerboseAssertLt buffer fully used!");
        },
        val1_str, val2_str, file, line
    );
}

// ------------------------------
// LE (Less Than or Equal) assert base
// ------------------------------

template<class Val1T, class Val2T>
void VerboseAssertLe(const Val1T &val1,
                     const Val2T &val2,
                     const char *val1_str,
                     const char *val2_str,
                     const char *file,
                     const char *line) {
    VerboseAssertTwoArgBase(
        val1, val2,
        [](const Val1T &v1, const Val2T &v2) { return v1 <= v2; },
        [](char *msg, const int size, const char *v1_str, const char *v2_str,
           const char *v1_dump, const char *v2_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (LE)!\n"
                                               "First value should be less than or equal to second value!\n"
                                               "First value: %s\n"
                                               "Which is: %s\n"
                                               "Second value: %s\n"
                                               "Which is: %s\n", v1_str, v1_dump, v2_str, v2_dump);
            ASSERT(bytes_written < size && "VerboseAssertLe buffer fully used!");
        },
        val1_str, val2_str, file, line
    );
}

// ------------------------------
// GT (Greater Than) assert base
// ------------------------------

template<class Val1T, class Val2T>
void VerboseAssertGt(const Val1T &val1,
                     const Val2T &val2,
                     const char *val1_str,
                     const char *val2_str,
                     const char *file,
                     const char *line) {
    VerboseAssertTwoArgBase(
        val1, val2,
        [](const Val1T &v1, const Val2T &v2) { return v1 > v2; },
        [](char *msg, const int size, const char *v1_str, const char *v2_str,
           const char *v1_dump, const char *v2_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (GT)!\n"
                                               "First value should be greater than second value!\n"
                                               "First value: %s\n"
                                               "Which is: %s\n"
                                               "Second value: %s\n"
                                               "Which is: %s\n", v1_str, v1_dump, v2_str, v2_dump);
            ASSERT(bytes_written < size && "VerboseAssertGt buffer fully used!");
        },
        val1_str, val2_str, file, line
    );
}

// ------------------------------
// GE (Greater Than or Equal) assert base
// ------------------------------

template<class Val1T, class Val2T>
void VerboseAssertGe(const Val1T &val1,
                     const Val2T &val2,
                     const char *val1_str,
                     const char *val2_str,
                     const char *file,
                     const char *line) {
    VerboseAssertTwoArgBase(
        val1, val2,
        [](const Val1T &v1, const Val2T &v2) { return v1 >= v2; },
        [](char *msg, const int size, const char *v1_str, const char *v2_str,
           const char *v1_dump, const char *v2_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (GE)!\n"
                                               "First value should be greater than or equal to second value!\n"
                                               "First value: %s\n"
                                               "Which is: %s\n"
                                               "Second value: %s\n"
                                               "Which is: %s\n", v1_str, v1_dump, v2_str, v2_dump);
            ASSERT(bytes_written < size && "VerboseAssertGe buffer fully used!");
        },
        val1_str, val2_str, file, line
    );
}

// ------------------------------
// String Equal assert base
// ------------------------------

inline void VerboseAssertStrEq(const char *val1,
                               const char *val2,
                               const char *val1_str,
                               const char *val2_str,
                               const char *file,
                               const char *line) {
    VerboseAssertTwoArgBase(
        val1, val2,
        [](const char *v1, const char *v2) { return strcmp(v1, v2) == 0; },
        [](char *msg, const int size, const char *v1_str, const char *v2_str,
           const char *v1_dump, const char *v2_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (STREQ)!\n"
                                               "Strings should be equal!\n"
                                               "First string: %s\n"
                                               "Which is: %s\n"
                                               "Second string: %s\n"
                                               "Which is: %s\n", v1_str, v1_dump, v2_str, v2_dump);
            ASSERT(bytes_written < size && "VerboseAssertStrEq buffer fully used!");
        },
        val1_str, val2_str, file, line
    );
}

// ------------------------------
// String Not Equal assert base
// ------------------------------

inline void VerboseAssertStrNeq(const char *val1,
                                const char *val2,
                                const char *val1_str,
                                const char *val2_str,
                                const char *file,
                                const char *line) {
    VerboseAssertTwoArgBase(
        val1, val2,
        [](const char *v1, const char *v2) { return strcmp(v1, v2) != 0; },
        [](char *msg, const int size, const char *v1_str, const char *v2_str,
           const char *v1_dump, const char *v2_dump) {
            const int bytes_written = snprintf(msg, size,
                                               "Assertion failed (STRNEQ)!\n"
                                               "Strings should not be equal!\n"
                                               "First string: %s\n"
                                               "Which is: %s\n"
                                               "Second string: %s\n"
                                               "Which is: %s\n", v1_str, v1_dump, v2_str, v2_dump);
            ASSERT(bytes_written < size && "VerboseAssertStrNeq buffer fully used!");
        },
        val1_str, val2_str, file, line
    );
}


// ------------------------------
// Macro wrappers
// ------------------------------

#define BASE_ASSERT_EQ(is_active, expected, value) if constexpr (is_active) VerboseAssertEq(expected, value, TOSTRING(expected), TOSTRING(value), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_NEQ(is_active, expected, value) if constexpr (is_active) VerboseAssertNeq(expected, value, TOSTRING(expected), TOSTRING(value), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_TRUE(is_active, value) if constexpr (is_active) VerboseAssertTrue(value, TOSTRING(value), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_FALSE(is_active, value) if constexpr (is_active) VerboseAssertFalse(value, TOSTRING(value), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_NOT_NULL(is_active, value) if constexpr (is_active) VerboseAssertNotNull(value, TOSTRING(value), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_NULL(is_active, value) if constexpr (is_active) VerboseAssertNull(value, TOSTRING(value), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_LT(is_active, val1, val2) if constexpr (is_active) VerboseAssertLt(val1, val2, TOSTRING(val1), TOSTRING(val2), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_LE(is_active, val1, val2) if constexpr (is_active) VerboseAssertLe(val1, val2, TOSTRING(val1), TOSTRING(val2), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_GT(is_active, val1, val2) if constexpr (is_active) VerboseAssertGt(val1, val2, TOSTRING(val1), TOSTRING(val2), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_GE(is_active, val1, val2) if constexpr (is_active) VerboseAssertGe(val1, val2, TOSTRING(val1), TOSTRING(val2), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_STREQ(is_active, val1, val2) if constexpr (is_active) VerboseAssertStrEq(val1, val2, TOSTRING(val1), TOSTRING(val2), __FILE__, TOSTRING(__LINE__))
#define BASE_ASSERT_STRNEQ(is_active, val1, val2) if constexpr (is_active) VerboseAssertStrNeq(val1, val2, TOSTRING(val1), TOSTRING(val2), __FILE__, TOSTRING(__LINE__))


// ------------------------------
// Verbose asserts
// ------------------------------

/* usual C-style asserts */
#define ASSERT_EQ(expected, value) BASE_ASSERT_EQ(kIsDebugBuild, expected, value)
#define ASSERT_NEQ(expected, value) BASE_ASSERT_NEQ(kIsDebugBuild, expected, value)
#define ASSERT_TRUE(value) BASE_ASSERT_TRUE(kIsDebugBuild, value)
#define ASSERT_FALSE(value) BASE_ASSERT_FALSE(kIsDebugBuild, value)
#define ASSERT_NOT_NULL(value) BASE_ASSERT_NOT_NULL(kIsDebugBuild, value)
#define ASSERT_NULL(value) BASE_ASSERT_NULL(kIsDebugBuild, value)
#define ASSERT_LT(val1, val2) BASE_ASSERT_LT(kIsDebugBuild, val1, val2)
#define ASSERT_LE(val1, val2) BASE_ASSERT_LE(kIsDebugBuild, val1, val2)
#define ASSERT_GT(val1, val2) BASE_ASSERT_GT(kIsDebugBuild, val1, val2)
#define ASSERT_GE(val1, val2) BASE_ASSERT_GE(kIsDebugBuild, val1, val2)
#define ASSERT_STREQ(val1, val2) BASE_ASSERT_STREQ(kIsDebugBuild, val1, val2)
#define ASSERT_STRNEQ(val1, val2) BASE_ASSERT_STRNEQ(kIsDebugBuild, val1, val2)


/* release build asserts */
#define R_ASSERT_EQ(expected, value) BASE_ASSERT_EQ(true, expected, value)
#define R_ASSERT_NEQ(expected, value) BASE_ASSERT_NEQ(true, expected, value)
#define R_ASSERT_TRUE(value) BASE_ASSERT_TRUE(true, value)
#define R_ASSERT_FALSE(value) BASE_ASSERT_FALSE(true, value)
#define R_ASSERT_NOT_NULL(value) BASE_ASSERT_NOT_NULL(true, value)
#define R_ASSERT_NULL(value) BASE_ASSERT_NULL(true, value)
#define R_ASSERT_LT(val1, val2) BASE_ASSERT_LT(true, val1, val2)
#define R_ASSERT_LE(val1, val2) BASE_ASSERT_LE(true, val1, val2)
#define R_ASSERT_GT(val1, val2) BASE_ASSERT_GT(true, val1, val2)
#define R_ASSERT_GE(val1, val2) BASE_ASSERT_GE(true, val1, val2)
#define R_ASSERT_STREQ(val1, val2) BASE_ASSERT_STREQ(true, val1, val2)
#define R_ASSERT_STRNEQ(val1, val2) BASE_ASSERT_STRNEQ(true, val1, val2)

#endif  // KERNEL_INCLUDE_ASSERT_HPP_
