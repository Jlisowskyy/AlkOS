/* internal includes */
#include <arch_utils.hpp>
#include <kernel_assert.hpp>
#include <terminal.hpp>
#include <test_module/test_module.hpp>

TODO_BY_THE_END_OF_MILESTONE0
static int strcmp(const char *str1, const char *str2)
{
    size_t i       = 0;
    const auto *s1 = reinterpret_cast<const unsigned char *>(str1);
    const auto *s2 = reinterpret_cast<const unsigned char *>(str2);
    while (s1[i] == s2[i] && s1[i]) i++;
    return s1[i] - s2[i];
}

namespace test
{

char g_testMem[kStubMemSize]{};
TestSpec g_tests[kMaxTests]{};
u32 g_numTests{};
bool g_expectFail{};
bool g_testCheckFailed{};
bool g_testStarted{};

void TestModule::RunTestModule()
{
    static constexpr size_t kInputBufferSize = 512;

    /* display all possible tests */
    DisplayTests_();

    /* read single line of input */
    char buff[kInputBufferSize];
    TerminalWriteString("Provide test name for framework or simply type \"exit\" to quit...\n");
    if (TerminalReadLine(buff, kInputBufferSize) == kInputBufferSize) {
        TerminalWriteError("[TEST] [FAIL] Too long input message...\n");
        QemuShutdown();
    }

    if (strcmp(buff, "exit") == 0) {
        TerminalWriteString("Kernel shutdown on test...\n");
        QemuShutdown();
    }

    TestSpec *test = FindTestFunction(buff);

    if (test == nullptr) {
        TerminalWriteError("[TEST] [FAIL] Test not found...\n");
        QemuShutdown();
    }

    RunTest_(test);
    QemuShutdown();
}

void TestModule::DisplayTests_()
{
    TerminalWriteString("Displaying list of all tests:\n");
    for (size_t idx = 0; idx < g_numTests; ++idx) {
        TerminalWriteString("[TEST] [TESTNAME] ");
        TerminalWriteString(g_tests[idx].name);
        TerminalWriteString("\n");
    }

    TerminalWriteString("[TEST] [LISTEND]\n");
}

TestSpec *TestModule::FindTestFunction(const char *name)
{
    for (size_t idx = 0; idx < g_numTests; ++idx) {
        if (strcmp(name, g_tests[idx].name) == 0) {
            return g_tests + idx;
        }
    }

    return nullptr;
}

void TestModule::RunTest_(const TestSpec *test)
{
    TerminalWriteString("Running test:\n");
    TerminalWriteString(test->name);
    TerminalWriteString("\n");

    TestGroupBase *test_obj = test->factory(g_testMem);
    ASSERT_NOT_NULL(test_obj);

    /* mark that the test is already started */
    g_testStarted = true;
    test_obj->Run();

    if (g_expectFail) {
        /* test was expected to fail */
        TerminalWriteError(
            "[TEST] [FAIL] Test was supposed to fail but all checks passed correctly...\n"
        );
        return;
    }

    if (g_testCheckFailed) {
        TerminalWriteError("[TEST] [FAIL] Test failed on some EXPECT_* checks...\n");
        return;
    }

    TerminalWriteString("[TEST] [SUCCESS] Test correctly passed...\n");
}

void AddTest(const char *name, const test_factory_t factory)
{
    ASSERT_NULL(TestModule::FindTestFunction(name));
    TestSpec *pTestSpec = &g_tests[g_numTests++];

    pTestSpec->factory = factory;
    pTestSpec->name    = name;
}

void OnKernelPanic()
{
    if (!g_testStarted) {
        /* test not started yet, but we received failure -> some critical bug -> abort execution */
        TerminalWriteError("[TEST] [FAIL] Kernel panic received before test started...\n");
        QemuShutdown();
    }

    if (g_expectFail) {
        /* fail was expected we are goood af */
        TerminalWriteString("[TEST] [SUCCESS] Test failed successfully...\n");
        QemuShutdown();
    }

    /* ups... */
    TerminalWriteError("[TEST] [FAIL] Test failed on kernel panic...\n");
    QemuShutdown();
}

}  // namespace test
