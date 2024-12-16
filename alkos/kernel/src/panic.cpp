#include <panic.hpp>

extern "C" void terminal_write_error(const char *data);

extern "C" void os_hang();

extern "C" void KernelPanic(const char *msg) {
    terminal_write_error(msg);
    os_hang();
}
