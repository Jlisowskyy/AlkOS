#include <comp.h>

extern "C" void MULTITARGET_FUNC(TerminalWriteError)(const char *data);
extern "C" void MULTITARGET_FUNC(os_hang)();

DEF_MULTITARGET_FUNC(void, KernelPanic, (const char *msg)) {
    MULTITARGET_FUNC(TerminalWriteError)(msg);
    MULTITARGET_FUNC(os_hang)();
}
