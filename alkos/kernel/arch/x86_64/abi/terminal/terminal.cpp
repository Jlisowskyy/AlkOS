/* internal includes */
#include <defines.hpp>
#include <terminal.hpp>

#include <serial_port_qemu/serial_qemu.hpp>
#include <vga/vga.hpp>
#include <todo.hpp>

extern "C" {
void TerminalInit()
{
    /* Initialize VGA terminal -> when multiboot allows: TODO */
    // VgaTerminalInit();

    if constexpr (kUseDebugOutput) {
        QemuTerminalInit();
    }
}

void TerminalPutChar(const char c)
{
    /* Put char to VGA terminal -> when multiboot allows: TODO */
    // VgaTerminalPutChar(c);

    if constexpr (kUseDebugOutput) {
        QemuTerminalPutChar(c);
    }
}

void TerminalWriteString(const char *data)
{
    /* Write string to VGA terminal -> when multiboot allows: TODO */
    // VgaTerminalWriteString(data);

    if constexpr (kUseDebugOutput) {
        QemuTerminalWriteString(data);
    }
}

void TerminalWriteError(const char *data)
{
    /* Write error string to VGA terminal -> when multiboot allows: TODO */
    // VgaTerminalWriteError(data);

    if constexpr (kUseDebugOutput) {
        QemuTerminalWriteString(data);
    }
}

char TerminalGetChar()
{
    if constexpr (kUseDebugOutput) {
        return QemuTerminalGetChar();
    }

    TODO_BY_THE_END_OF_MILESTONE0
    return 'x';
}

size_t TerminalReadLine(char *buffer, const size_t size)
{
    if constexpr (kUseDebugOutput) {
        return QemuTerminalReadLine(buffer, size);
    }

    TODO_BY_THE_END_OF_MILESTONE0
    return 0;
}
}
