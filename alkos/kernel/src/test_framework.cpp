/* internal includes */
#include <arch_utils.hpp>
#include <terminal.hpp>
#include <test_framework.hpp>

TODO_BY_THE_END_OF_MILESTONE0
static int strcmp(const char *str1, const char *str2)
{
    size_t i       = 0;
    const auto *s1 = reinterpret_cast<const unsigned char *>(str1);
    const auto *s2 = reinterpret_cast<const unsigned char *>(str2);
    while (s1[i] == s2[i] && s1[i]) i++;
    return s1[i] - s2[i];
}

void TestFramework::RunTestModule()
{
    static constexpr size_t kInputBufferSize = 512;

    /* display all possible tests */
    TerminalWriteString("Displaying list of all tests:\n");

    /* TODO */
    TerminalWriteString("[TEST] [TESTNAME] a\n");
    TerminalWriteString("[TEST] [TESTNAME] b\n");
    TerminalWriteString("[TEST] [TESTNAME] c\n");
    TerminalWriteString("[TEST] [LISTEND]\n");

    /* read single line of input */
    char buff[kInputBufferSize];
    TerminalWriteString("Provide test name for framework or simply type \"exit\" to quit...\n");
    if (TerminalReadLine(buff, kInputBufferSize) == kInputBufferSize) {
        TerminalWriteError("[TEST] [FAIL] Too long input message...\n");
        QemuShutdown();
    }

    /* TODO: strcmp */
    TODO_BY_THE_END_OF_MILESTONE0
    if (strcmp(buff, "exit") == 0) {
        TerminalWriteString("Kernel shutdown on test...\n");
        QemuShutdown();
    }

    const test_t test = FindTestFunction_(buff);

    if (test == nullptr) {
        TerminalWriteError("[TEST] [FAIL] Test not found...\n");
        QemuShutdown();
    }

    RunTest_(test);
    QemuShutdown();
}

TestFramework::test_t TestFramework::FindTestFunction_(const char *name)
{
    TODO_BY_THE_END_OF_MILESTONE0
    return reinterpret_cast<test_t>(0xDEAD);
}

void TestFramework::RunTest_(test_t test) { TerminalWriteString("[TEST] [SUCCESS]\n"); }
