#include <comp.h>

extern "C" void MULTITARGET_FUNC(TerminalWriteError)(const char *data);

DEF_MULTITARGET_FUNC(__attribute__((__noreturn__)) void, KernelPanic, (const char *msg)) {
    MULTITARGET_FUNC(TerminalWriteError)(msg);

    while (1) {
        asm volatile("cli");
        asm volatile("hlt");
    }

    MULTITARGET_FUNC(TerminalWriteError)("XDDD");
}
