#ifndef KERNEL_INCLUDE_DEBUG_TERMINAL_HPP_
#define KERNEL_INCLUDE_DEBUG_TERMINAL_HPP_

#include <defines.hpp>
#include <types.hpp>

/* Should be implement inside architecture code */
extern "C"
{
    size_t DebugTerminalReadLineArch_(char *buffer, size_t buffer_size);
    void DebugTerminalWriteArch_(const char *buffer);
}

WRAP_CALL void DebugTerminalWrite(const char *str)
{
    if constexpr (kUseDebugOutput)
    {
        DebugTerminalWriteArch_(str);
    }
}

WRAP_CALL size_t DebugTerminalReadLine(char *const buffer, const size_t buffer_size)
{
    if constexpr (kUseDebugOutput)
    {
        return DebugTerminalReadLineArch_(buffer, buffer_size);
    }

    return 0;
}

#endif // KERNEL_INCLUDE_DEBUG_TERMINAL_HPP_
