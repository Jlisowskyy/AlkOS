/* internal includes */

#include <arch_utils.hpp>
#include <panic.hpp>
#include <terminal.hpp>
#include <test_module/test_module.hpp>

extern "C" void KernelPanic(const char *msg)
{
    TerminalWriteError("[ KERNEL PANIC ]\n");
    TerminalWriteError(msg);
    TerminalWriteError("\n");

// #TODO
//#ifdef __x86_64__
//    if constexpr (kIsAlkosTestBuild) {
//        /* When running tests */
//        test::OnKernelPanic();
//    } else {
//        /* Usual situation */
//
//        OsHang();
//    }
//#else
//    OsHang();
//#endif
}
