/* internal includes */
#include <panic.hpp>
#include <terminal.hpp>

extern "C" void KernelPanic(const char *msg) {
    TerminalWriteError("[ KERNEL PANIC ]\n");
    TerminalWriteError(msg);

    asm volatile("cli");
    while (1) {
        asm volatile("hlt");
    }
}
