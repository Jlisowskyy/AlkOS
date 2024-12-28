#ifndef ALKOS_TEST_TESTER_HPP_
#define ALKOS_TEST_TESTER_HPP_

#include <defines.hpp>
#include <types.hpp>

enum TestType : uint64_t {
    kStackSmashTest,
    kCppTest,
    kFloatExtensionTest,
    kExceptionTest,
    kSerialInTest,
    kPreserveCpuStateTest,
    kLastTest,
};

#ifdef ALKOS_TEST

#if (0 - ALKOS_TEST - 1) == 1
#error "Test type is invalid: empty ALKOS_TEST macro"
#endif

#endif

#define VERIFY_TEST_TYPE(type)                                                \
    static_assert(                                                            \
        type < TestType::kLastTest,                                           \
        "Test type is invalid. File: " __FILE__ ", Line: " TOSTRING(__LINE__) \
    );

void RunTest(TestType type);

#endif  // ALKOS_TEST_TESTER_HPP_
