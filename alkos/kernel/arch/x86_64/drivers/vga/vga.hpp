#ifndef KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_

/* internal includes */
#include <comp.h>

DEF_MULTITARGET_FUNC(void, VgaTerminalInit, ());

DEF_MULTITARGET_FUNC(void, VgaTerminalSetColor, (uint8_t color));

DEF_MULTITARGET_FUNC(void, VgaTerminalPutEntryAt, (char c, uint8_t color, uint32_t x,uint32_t y));

DEF_MULTITARGET_FUNC(void, VgaTerminalPutChar, (char c));

DEF_MULTITARGET_FUNC(void, VgaTerminalWrite, (const char *data, uint32_t size));

DEF_MULTITARGET_FUNC(void, VgaTerminalWriteString, (const char *data));

DEF_MULTITARGET_FUNC(void, VgaTerminalWriteError, (const char *data));

#endif // KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_
