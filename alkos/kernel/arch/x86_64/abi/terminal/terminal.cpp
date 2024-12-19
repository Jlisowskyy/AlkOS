/* internal includes */
#include <terminal.hpp>
#include <defines.hpp>

#ifdef __SERIAL_PORT_TEST__
#include <serial_port_qemu/serial_qemu.hpp>
#endif // __SERIAL_PORT_TEST__

#include <vga/vga.hpp>

extern "C" {
    void TerminalInit() {
        /* Initialize VGA terminal -> when multiboot allows: TODO */
        // VgaTerminalInit();

        if constexpr (kSerialPortTest) {
            QemuTerminalInit();
        }
    }

    void TerminalPutChar(const char c) {
        /* Put char to VGA terminal -> when multiboot allows: TODO */
        // VgaTerminalPutChar(c);

        if constexpr (kSerialPortTest) {
            QemuTerminalPutChar(c);
        }
    }

    void TerminalWriteString(const char *data) {
        /* Write string to VGA terminal -> when multiboot allows: TODO */
        // VgaTerminalWriteString(data);

        if constexpr (kSerialPortTest) {
            QemuTerminalWriteString(data);
        }
    }

    void TerminalWriteError(const char *data) {
        /* Write error string to VGA terminal -> when multiboot allows: TODO */
        // VgaTerminalWriteError(data);

        if constexpr (kSerialPortTest) {
            QemuTerminalWriteString(data);
        }
    }
}

