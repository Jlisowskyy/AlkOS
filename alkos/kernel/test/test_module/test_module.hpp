#ifndef ALKOS_KERNEL_TEST_TEST_MODULE_TEST_MODULE_HPP_
#define ALKOS_KERNEL_TEST_TEST_MODULE_TEST_MODULE_HPP_

#include <test_module/test_group_base.hpp>
#include <todo.hpp>
#include <types.hpp>

namespace test
{

/* Whole test framework should be reworked when threads are available */
TODO_WHEN_MULTITHREADING

/* 64 MB memory for tests framework - used because of lack of vmem */
TODO_WHEN_VMEM_WORKS
static constexpr u32 kStubMemSize = 64 * 1024 * 1024;

/* size of test array  */
static constexpr u32 kMaxTests = 4096;

/* should use given memory to construct test object - used due to lack of vmem */
using test_factory_t = TestGroupBase* (*)(void*);

struct TestSpec {
    const char* name;
    test_factory_t factory;
};

class TestModule final
{
    // ------------------------------
    // Class creation
    // ------------------------------

    public:
    TestModule()  = default;
    ~TestModule() = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void RunTestModule();

    // ------------------------------
    // Class private methods
    // ------------------------------

    private:
    static void DisplayTests_();
    static TestSpec* FindTestFunction_(const char* name);
    static void RunTest_(const TestSpec* test);

    // ------------------------------
    // Class fields
    // ------------------------------
};

void AddTest(const char* name, test_factory_t factory);

TODO_BY_THE_END_OF_MILESTONE0
extern char g_testMem[kStubMemSize];

TODO_BY_THE_END_OF_MILESTONE1
/* TODO: replace with some handy structures */

/* array of defined tests, each test should automatically add himself here */
extern TestSpec g_tests[kMaxTests];

/* counter of added tests */
extern u32 g_numTests;

/* this flag controls whether given test case should fail - in this case fail means total failure on
 * some assert */
extern bool g_expectFail;

/* this flag controls whether during the test execution some EXPECT_* check failed */
extern bool g_testCheckFailed;

}  // namespace test

#endif  // ALKOS_KERNEL_TEST_TEST_MODULE_TEST_MODULE_HPP_
