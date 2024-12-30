/**
 * Implementation of serial port communication for QEMU.
 * For detailed implementation reference, see: https://wiki.osdev.org/Serial_Ports
 */

/* internal includes */
#include <debug.hpp>
#include <io.hpp>
#include <kernel_assert.hpp>
#include <serial_port_qemu/serial_qemu.hpp>

// ------------------------------
// Port and Register Definitions
// ------------------------------

static constexpr u16 kCom1Port                       = 0x3f8;
static constexpr u16 kInterruptEnableReg             = kCom1Port + 1;
static constexpr u16 kReadInterruptIdentificationReg = kCom1Port + 2;
static constexpr u16 kWriteFifoControlReg            = kCom1Port + 2;
static constexpr u16 kLineControlReg                 = kCom1Port + 3;
static constexpr u16 kModemControlReg                = kCom1Port + 4;
static constexpr u16 kReadLineStatusReg              = kCom1Port + 5;
static constexpr u16 kReadModemStatusReg             = kCom1Port + 6;
static constexpr u16 kScratchReg                     = kCom1Port + 7;

static constexpr byte kReceivedBit = 1; /* Indicates data has been received */
static constexpr byte kDlabBit =
    0x80; /* Divisor Latch Access Bit - controls access to baud rate divisor */
static constexpr byte kLineEmpty = 0x20; /* Indicates transmit buffer is empty */

// ------------------------------
// Local Helper Functions
// ------------------------------

WRAP_CALL bool SerialReceived() { return (inb(kReadLineStatusReg) & kReceivedBit) != 0; }

WRAP_CALL bool IsLineEmpty() { return (inb(kReadLineStatusReg) & kLineEmpty) != 0; }

// ------------------------------
// Global Interface Functions
// ------------------------------

extern "C" {
void QemuTerminalInit()
{
    /**
     * Line Control Configuration:
     * - Data bits [0-1]: Set to 11 (8 bits per transmission)
     * - Stop bit (2): Set to 0 (single stop bit)
     * - Parity bits [3-5]: Set to 000 (no parity)
     * - DLAB (6) and Break Enable (7): Both disabled
     */
    static constexpr byte kLineControlConfFlags = 0x3;

    /**
     * FIFO Control Configuration (0xC7 = 0b1100'0111):
     * - Bits [6-7]: Set to 11 (14-byte FIFO interrupt threshold)
     * - Bit 2: Set to 1 (clear transmit FIFO)
     * - Bit 1: Set to 1 (clear receive FIFO)
     * - Bit 0: Set to 1 (enable FIFO mode)
     */
    static constexpr byte kFifoConfFlags = 0xC7;

    /**
     * Modem Control Configuration (0x0B = 0b0000'1011):
     * - Bit 0 (DTR): Set to 1 (Data Terminal Ready enabled)
     * - Bit 1 (RTS): Set to 1 (Request To Send enabled)
     * - Bit 3: Set to 1 (Enable IRQs)
     * - Bit 4: Set to 0 (Disable loopback)
     * - Bits [2,5-7]: Unused
     */
    static constexpr byte kModemConfFlags = 0x0B;

    /**
     * Loopback Modem Configuration (0x1A = 0b0001'1010):
     * - Bit 0 (DTR): Set to 0 (Data Terminal Ready disabled)
     * - Bit 1 (RTS): Set to 1 (Request To Send enabled)
     * - Bit 3: Set to 1 (Enable IRQs)
     * - Bit 4: Set to 1 (Enable loopback)
     * - Bits [2,5-7]: Unused
     */
    static constexpr byte kLoopbackModemConfFlags = 0x1A;

    /* Test byte for loopback verification */
    static constexpr byte kTestByte = 0xAE;

    /* Temporarily disable interrupts */
    outb(kInterruptEnableReg, 0);

    /* ------------------------------------------------------------ */
    /* Configure baud rate divisor - must match the receiving end */

    /* Set DLAB to access baud rate divisor registers */
    outb(kLineControlReg, kDlabBit);

    /* Set baud rate divisor to 3 (38400 baud)
     * Lower byte at base port, upper byte at base port + 1 */
    outb(kCom1Port + 0, 3); /* Lower byte */
    outb(kCom1Port + 1, 0); /* Upper byte */

    /* ------------------------------------------------------------ */

    /* Apply line control configuration */
    outb(kLineControlReg, kLineControlConfFlags);

    /* Initialize FIFO with configuration */
    outb(kFifoConfFlags, kFifoConfFlags);

    /* -------------------------------------------------------------- */
    /* Verify port configuration using loopback test */

    /* Enable loopback mode for testing */
    outb(kModemControlReg, kLoopbackModemConfFlags);

    /* Send test byte through loopback */
    outb(kCom1Port, kTestByte);

    /* Verify received byte matches sent byte */
    R_ASSERT_EQ(kTestByte, inb(kCom1Port));

    /* -------------------------------------------------------------- */

    /* Switch to normal operation mode with final modem configuration */
    outb(kModemControlReg, kModemConfFlags);

    TRACE_SUCCESS("QemuTerminalInit() returned with success");
}

void QemuTerminalPutChar(const char c)
{
    while (!IsLineEmpty()) {
    }

    outb(kCom1Port, c);
}

void QemuTerminalWriteString(const char *s)
{
    while (*s) {
        QemuTerminalPutChar(*s);
        ++s;
    }
}

char QemuTerminalGetChar()
{
    /* Wait until data is available to read */
    while (!SerialReceived()) {
    }

    return static_cast<char>(inb(kCom1Port));
}

size_t QemuTerminalReadLine(char *buffer, const size_t size)
{
    for (size_t cur = 0; cur < size - 1; ++cur) {
        const char c = QemuTerminalGetChar();

        if (c == '\r') {
            buffer[cur] = '\0';
            return cur + 1;
        }

        buffer[cur] = c;
    }

    buffer[size - 1] = '\0';
    return size;
}
}
