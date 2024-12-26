/**
 * Refer to https://wiki.osdev.org/Serial_Ports for more details on implementation
 */

/* internal includes */
#include <debug.hpp>
#include <io.hpp>
#include <kernel_assert.hpp>
#include <serial_port_qemu/serial_qemu.hpp>
#include <temp.hpp>

// ------------------------------
// Crucial defines
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

static constexpr byte kReceivedBit = 1;
static constexpr byte kDlabBit     = 0x80; /* most significant bit of Line Control Reg */
static constexpr byte kLineEmpty   = 0x20;

// ------------------------------
// Local functions
// ------------------------------

WRAP_CALL bool SerialReceived() { return (inb(kReadLineStatusReg) & kReceivedBit) != 0; }

WRAP_CALL bool IsLineEmpty() { return (inb(kReadLineStatusReg) & kLineEmpty) != 0; }

// ------------------------------
// Global functions
// ------------------------------

void QemuTerminalInit()
{
    /**
     * This flags states that:
     * - we send 8 bits at once (data bits [0-1] set to 11)
     * - we use single stop bit (stop bits (2) set to 0)
     * - we use no parity bits  (parity bits [3-5] set to 0
     * - DLAB and Break Enable Bit disabled (bits in range [6-7])
     */
    static constexpr byte kLineControlConfFlags = 0x3;

    /**
     * 0xC=0b1100, 0x7=0b0111, 0xC7=0b1100'0111
     *
     * This flags states that:
     * - set up that after received 14 bytes in the FIFO interrupt will be triggered (bits [6-7] set to 11)
     * - clears transmit FIFO (bit 2)
     * - clears receive FIFO (bit 1)
     * - enables FIFO based communication (bit 0)
     */
    static constexpr byte kFifoConfFlags = 0xC7;

    /**
     * 0x0B = 0b0000'1011
     *
     * This flags states that:
     * - Data Terminal Ready (DTR) enabled (bit (0) set to 1)
     * - Request To Send (RTS) enabled (bit (1) set to 1)
     * - IRQs enables (bit (3) set to 1)
     * - loopback disabled (bit (4), bits[5-7] and (2) are unused)
     */
    static constexpr byte kModemConfFlags = 0x0B;

    /**
     * 0x1A = 0b0001'1010
     *
     * This flags states that:
     * - Data Terminal Ready (DTR) is disabled (bit (0) set to 0)
     * - Request To Send (RTS) enabled (bit (1) set to 1)
     * - IRQs enables (bit (3) set to 1)
     * - loopback is enabled (bit (4) set to 1)
     * - other bits are unused
     */
    static constexpr byte kLoopbackModemConfFlags = 0x1A;

    /* Send to the loopback, received should be same */
    static constexpr byte kTestByte = 0xAE;

    /* disable interrupts - TODO: for now? */
    outb(kInterruptEnableReg, 0);

    /* ------------------------------------------------------------ */
    /* Configure baud rate divisor - must be same as the other site */

    /* enable DLAB (Divisor Latch Access Bit) - msb of LCR */
    outb(kLineControlReg, kDlabBit);

    /* set divisor to 3 (38400 baud rate), with dlab enabled
     * com1 + 0 - lo byte, com1 + 1 - hi byte */
    outb(kCom1Port + 0, 3); /* lo byte */
    outb(kCom1Port + 1, 0); /* hi byte */

    /* ------------------------------------------------------------ */

    /* configure specification of communication */
    outb(kLineControlReg, kLineControlConfFlags);

    /* apply fifo configuration */
    outb(kFifoConfFlags, kFifoConfFlags);

    /* -------------------------------------------------------------- */
    /* Verify correctness of configuration by writing to the loopback */

    /* configure loopback */
    outb(kModemControlReg, kLoopbackModemConfFlags);

    /* send payload */
    outb(kCom1Port, kTestByte);

    /* received and verify payload */
    ASSERT_EQ(kTestByte, inb(kCom1Port));

    /* -------------------------------------------------------------- */

    /* finally load modem configuration */
    outb(kModemControlReg, kModemConfFlags);

    TRACE_SUCCESS("QemuTerminalInit() returned with success");
}

void QemuTerminalPutChar(const char c)
{
    while (!IsLineEmpty())
    {
    }

    outb(kCom1Port, c);
}

void QemuTerminalWriteString(const char *s)
{
    while (*s)
    {
        QemuTerminalPutChar(*s);
        ++s;
    }
}

char QemuTerminalGetChar()
{
    /* wait for payload */
    while (!SerialReceived())
    {
    }

    return static_cast<char>(inb(kCom1Port));
}

size_t QemuTerminalReadLine(char *buffer, const size_t size)
{
    for (size_t cur = 0; cur < size - 1; ++cur)
    {
        const char c = QemuTerminalGetChar();
        if (c == '\r')
        {
            buffer[cur] = '\0';
            return cur + 1;
        }
        
        buffer[cur] = c;
    }

    buffer[size - 1] = '\0';
    return size;
}
