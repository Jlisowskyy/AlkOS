/* internal includes */
#include <io.hpp>
#include <serial_port_qemu/serial_qemu.hpp>

// ------------------------------
// Crucial defines
// ------------------------------

static constexpr u16 kCom1Port     = 0x3f8;
static constexpr byte kReceivedBit = 1;

// ------------------------------
// Local functions
// ------------------------------

WRAP_CALL bool SerialReceived() { return inb(kCom1Port + 5) & kReceivedBit; }

// ------------------------------
// Global functions
// ------------------------------

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

        if (c == '\n')
        {
            buffer[cur] = '\0';
            return cur + 1;
        }

        buffer[cur] = c;
    }

    buffer[size - 1] = '\0';
    return size;
}
