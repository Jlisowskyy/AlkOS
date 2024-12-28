#ifdef ALKOS_TEST
#include <tester.hpp>
#endif

/* internal includes */
#include <init.hpp>
#include <terminal.hpp>
#include <debug.hpp>

static void KernelRun() {
    TerminalWriteString("Hello from AlkOS!\n");
}

extern "C" void KernelMain() {
    TRACE_INFO("Running kernel initialization...");
    KernelInit();

#ifdef ALKOS_TEST

    TRACE_INFO("Running test: " TOSTRING(ALKOS_TEST));
    VERIFY_TEST_TYPE(ALKOS_TEST)
    RunTest(static_cast<TestType>(ALKOS_TEST));
    TerminalWriteString("Hello from AlkOS test!");
    return;

#endif

    TRACE_INFO("Proceeding to KernelRun...");
    KernelRun();
}
