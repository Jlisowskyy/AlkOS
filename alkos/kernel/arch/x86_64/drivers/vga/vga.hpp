#ifndef KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_

#include <types.hpp>

void VgaTerminalInit();
void VgaTerminalSetColor(uint8_t color);
void VgaTerminalPutEntryAt(char c, uint8_t color, size_t x, size_t y);
void VgaTerminalPutChar(char c);
void VgaTerminalWrite(const char *data, size_t size);
void VgaTerminalWriteString(const char *data);
void VgaTerminalWriteError(const char *data);

#endif // KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_
