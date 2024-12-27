/* internal includes */
#include <../arch/x86_64/include/arch_utils.hpp>
#include <../include/kernel_assert.hpp>
#include <../include/terminal.hpp>
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

TODO_BY_THE_END_OF_MILESTONE0
static size_t strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s);
    return s - str;
}

TODO_BY_THE_END_OF_MILESTONE0
static char *strncat(char *dest, const char *src, size_t n)
{
    size_t i = 0, j = 0;
    char *tmp = dest;
    tmp += strlen(dest);
    while (src[j] && j < n) {
        tmp[i++] = src[j++];
    }
    tmp[i] = '\0';
    return dest;
}

namespace test
{

char g_testMem[kStubMemSize]{};
TestSpec g_tests[kMaxTests]{};
u32 g_numTests{};
bool g_expectFail{};
bool g_testCheckFailed{};

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

    TestSpec *test = FindTestFunction_(buff);

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

TestSpec *TestModule::FindTestFunction_(const char *name)
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
    TestSpec *pTestSpec = &g_tests[g_numTests++];

    pTestSpec->factory = factory;
    pTestSpec->name    = name;
}

}  // namespace test
