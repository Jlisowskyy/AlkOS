/* internal includes */
#include <comp.h>
#include <vga/vga.hpp>
#include <constants.hpp>

LINK_MULTITARGET_FUNC(void, serial_init, ());

LINK_MULTITARGET_FUNC(void, serial_putchar, (char c));

LINK_MULTITARGET_FUNC(void, serial_puts, (const char* s));

DEF_MULTITARGET_FUNC(void, TerminalInit, ()) {
    /* Initialize VGA terminal -> when multiboot allows: TODO */
    // MULTITARGET_FUNC(VgaTerminalInit())();

    if constexpr (kSerialPortTest) {
        /* Initialize serial port if in QEMU targeting build*/
        MULTITARGET_FUNC(serial_init)();
    }
}

DEF_MULTITARGET_FUNC(void, TerminalPutChar, (char c)) {
    /* Put character to VGA terminal -> when mutliboot allows: TODO */
    // MULTITARGET_FUNC(VgaTerminalPutChar())(c);

    if constexpr (kSerialPortTest) {
        /* Put character to serial port if in QEMU targeting build */
        MULTITARGET_FUNC(serial_putchar)(c);
    }
}

DEF_MULTITARGET_FUNC(void, TerminalWriteString, (const char* data)) {
    /* Write string to VGA terminal -> when multiboot allows: TODO */
    // MULTITARGET_FUNC(VgaTerminalWriteString())(data);

    if constexpr (kSerialPortTest) {
        /* Write string to serial port if in QEMU targeting build */
        MULTITARGET_FUNC(serial_puts)(data);
    }
}

DEF_MULTITARGET_FUNC(void, TerminalWriteError, (const char* data)) {
    /* Write error message to VGA terminal -> when multiboot allows: TODO */
    // MULTITARGET_FUNC(VgaTerminalWriteError())(data);

    if constexpr (kSerialPortTest) {
        /* Write error message to serial port if in QEMU targeting build */
        MULTITARGET_FUNC(serial_puts)(data);
    }
}
