/* internal includes */
#include <arch_utils.hpp>
#include <panic.hpp>
#include <terminal.hpp>

extern "C" void KernelPanic(const char *msg)
{
    TerminalWriteError("[ KERNEL PANIC ]\n");
    TerminalWriteError(msg);

    OsHang();
}
