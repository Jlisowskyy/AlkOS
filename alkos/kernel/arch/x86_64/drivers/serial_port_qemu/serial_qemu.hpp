#ifndef KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_

#include <types.hpp>

void QemuTerminalInit();
void QemuTerminalPutChar(char c);
void QemuTerminalWriteString(const char *s);
char QemuTerminalGetChar();
size_t QemuTerminalReadLine(char *buffer, size_t size);

#endif // KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_
