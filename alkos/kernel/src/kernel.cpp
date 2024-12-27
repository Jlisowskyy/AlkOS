#ifdef ALKOS_TEST
#include <test_module/test_module.hpp>
#endif

/* internal includes */
#include <debug.hpp>
#include <init.hpp>
#include <terminal.hpp>

static void KernelRun() { TerminalWriteString("Hello from AlkOS!\n"); }

extern "C" void KernelMain()
{
    TRACE_INFO("Running kernel initialization...");
    KernelInit();

#ifdef ALKOS_TEST

    TRACE_INFO("Running tests...");
    test::TestModule test_module{};
    test_module.RunTestModule();

#endif

    TRACE_INFO("Proceeding to KernelRun...");
    KernelRun();
}
