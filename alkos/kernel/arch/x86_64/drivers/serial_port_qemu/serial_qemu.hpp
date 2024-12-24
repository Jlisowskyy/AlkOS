#ifndef KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_

#include <defines.hpp>
#include <types.hpp>

extern "C" void serial_init();

WRAP_CALL void QemuTerminalInit() {
    serial_init();
}

extern "C" void serial_putchar(char c);

WRAP_CALL void QemuTerminalPutChar(const char c) {
    serial_putchar(c);
}

extern "C" void serial_puts(const char *s);

WRAP_CALL void QemuTerminalWriteString(const char *s) {
    serial_puts(s);
}

char QemuTerminalGetChar();

size_t QemuTerminalReadLine(char *buffer, size_t size);

#endif // KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_
