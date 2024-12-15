#ifndef ALKOS_TEST_TESTER_HPP_
#define ALKOS_TEST_TESTER_HPP_

#include <stdint.h>

enum TestType: uint64_t {
    kStackSmashTest,
    kFloatExtensionTest,
    kLastTest,
};

#define IS_MACRO_EMPTY_(x) !(x##0)
#define IS_MACRO_EMPTY(x) IS_MACRO_EMPTY_(x)

#define VERIFY_TEST_TYPE(type)                                       \
static_assert(!IS_MACRO_EMPTY(type), "Test type is empty");          \
static_assert(type < TestType::kLastTest, "Test type is invalid");

void RunTest(TestType type);

#endif // ALKOS_TEST_TESTER_HPP_
