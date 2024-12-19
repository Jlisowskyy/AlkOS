/**
 * @file serial_32.cpp
 * @brief Serial port communication routines for 32-bit systems.
 *        Includes initialization, character output, and string output
 *        functions for COM1.
 */

#include <qemu_serial_port/qemu_serial_port.hpp>

/* internal includes */
#include <comp.h>

/* constants */
static constexpr uint16_t kSerialPortBaseCom1 = 0x3F8;
static constexpr uint16_t kDlabBit = 0x80;
static constexpr uint16_t kBaudDivisor = 3;
static constexpr uint16_t kEmpty = 0x20;
static constexpr uint16_t kNull = 0x00;

/* asm in/out functions */
static __attribute__((always_inline)) void outb(const uint16_t port, const uint8_t value) {
    asm volatile ("outb %b0, %w1" : : "a"(value), "Nd"(port));
}

static __attribute__((always_inline)) uint8_t inb(const uint16_t port) {
    uint8_t value;
    asm volatile ("inb %w1, %b0" : "=a"(value) : "Nd"(port));
    return value;
}

static __attribute__((always_inline)) bool IsTransmitEmpty() {
    return inb(kSerialPortBaseCom1 + 5) & kEmpty;
}

DEF_MULTITARGET_FUNC(void, QemuSerialPortInit, ()) {
    /* Disable interrupts: write 0 to IER */
    outb(kSerialPortBaseCom1 + 1, kNull);

    /* Set DLAB = 1 */
    outb(kSerialPortBaseCom1 + 3, kDlabBit);

    /* Set baud rate divisor to 38400 baud (divisor = 3) - lo byte */
    outb(kSerialPortBaseCom1 + 0, kBaudDivisor);

    /* Set baud rate divisor to 38400 baud (divisor = 3) - lhi byte */
    outb(kSerialPortBaseCom1 + 1, kNull);

    /* Clear DLAB and set data bits (8 bits, no parity, one stop bit) - LCR (COM1+3 = 0x3FB) */
    outb(kSerialPortBaseCom1 + 3, 0x3);

    /* Enable FIFO, clear them, 14-byte threshold: FCR (COM1+2 = 0x3FA) */
    outb(kSerialPortBaseCom1 + 2, 0xC7);

    /* Enable IRQs, RTS/DSR: MCR (COM1+4 = 0x3FC) */
    outb(kSerialPortBaseCom1 + 4, 0xB);
}

DEF_MULTITARGET_FUNC(void, QemuSerialPortPutChar, (const char c)) {
    while (!IsTransmitEmpty()) {
        /* Wait for transmit buffer to be empty */
    }

    outb(kSerialPortBaseCom1, static_cast<uint8_t>(c));
}

DEF_MULTITARGET_FUNC(void, QemuSerialPortPuts, (const char *str)) {
    while (*str) {
        MULTITARGET_FUNC(QemuSerialPortPutChar)(*str);
        ++str;
    }
}
