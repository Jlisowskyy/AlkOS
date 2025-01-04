/* internal includes */

#include <panic.hpp>
#include <terminal.hpp>

extern "C" void KernelPanic(const char *msg)
{
    TerminalWriteError("[ KERNEL PANIC ]\n");
    TerminalWriteError(msg);
    TerminalWriteError("\n");

    // #TODO
    // #ifdef __x86_64__
    //    if constexpr (kIsAlkosTestBuild) {
    //        /* When running tests */
    //        test::OnKernelPanic();
    //    } else {
    //        /* Usual situation */
    //
    //        OsHang();
    //    }
    // #else
    //    OsHang();
    // #endif
}
