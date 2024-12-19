#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__x86_64__)
#error "AlkOS needs to be compiled with a x86_64-elf compiler"
#endif

#ifdef ALKOS_TEST
#include <tester.hpp>
#endif

/* internal includes */
#include <init.hpp>
#include <terminal.hpp>
#include <debug.hpp>

static void KernelRun() {
    TerminalWriteString("Hello from AlkOS!");
}

extern "C" void KernelMain() {
    TRACE_INFO("Running kernel initialization...");
    KernelInit();

#ifdef ALKOS_TEST

    TRACE_INFO("Running tests: "##ALKOS_TEST);
    VERIFY_TEST_TYPE(ALKOS_TEST)
    RunTest(static_cast<TestType>(ALKOS_TEST));
    TerminalWriteString("Hello from AlkOS test!");
    return;

#endif

    TRACE_INFO("Proceeding to KernelRun...");
    KernelRun();
}
