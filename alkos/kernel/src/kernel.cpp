#ifdef __ALKOS_TEST__
#include <test_module/test_module.hpp>
#endif

/* internal includes */
#include <debug.hpp>
#include <init.hpp>
#include <kernel_assert.hpp>
#include <terminal.hpp>

static void KernelRun() { TerminalWriteString("Hello from AlkOS!\n"); }

extern "C" void KernelMain()
{
    TRACE_INFO("Running kernel initialization...");
    KernelInit();

#ifdef __ALKOS_TEST__

    TRACE_INFO("Running tests...");
    test::TestModule test_module{};
    test_module.RunTestModule();
    ASSERT_ALWAYS(false && "Test module should never exit!");

#endif

    TRACE_INFO("Proceeding to KernelRun...");
    KernelRun();
}
