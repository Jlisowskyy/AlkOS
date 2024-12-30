#include <test_module/test_module.hpp>

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

    if constexpr (kIsAlkosTestBuild) {
        TRACE_INFO("Running tests...");
        test::TestModule test_module{};
        test_module.RunTestModule();
        ASSERT_ALWAYS(false && "Test module should never exit!");
    }

    TRACE_INFO("Proceeding to KernelRun...");
    KernelRun();
}
