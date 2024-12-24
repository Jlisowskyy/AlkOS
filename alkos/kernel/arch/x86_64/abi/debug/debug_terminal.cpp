/* internal includes */
#include <debug_terminal.hpp>
#include <kernel_assert.hpp>
#include <serial_port_qemu/serial_qemu.hpp>

void DebugTerminalWriteArch_(const char *const buffer)
{
    /* verify if the usage is permitted */
    ASSERT_TRUE(kDebugTerminalEnabled);

    QemuTerminalWriteString(buffer);
}

size_t DebugTerminalReadLineArch_(char *const buffer, const size_t buffer_size)
{
    /* verify if the usage is permitted */
    ASSERT_TRUE(kDebugTerminalEnabled);

    return QemuTerminalReadLine(buffer, buffer_size);
}
