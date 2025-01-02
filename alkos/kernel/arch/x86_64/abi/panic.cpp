/* internal includes */

#include <arch_utils.hpp>
#include <panic.hpp>
#include <terminal.hpp>
#include <test_module/test_module.hpp>

extern "C" NO_RET void KernelPanic(const char *msg)
{
    TerminalWriteError("[ KERNEL PANIC ]\n");
    TerminalWriteError(msg);
    TerminalWriteError("\n");

    if constexpr (kIsAlkosTestBuild) {
        /* When running tests */
        test::OnKernelPanic();
    } else {
        /* Usual situation */

        OsHang();
    }
}
