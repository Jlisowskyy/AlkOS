#ifndef KERNEL_INCLUDE_TEMP_HPP_
#define KERNEL_INCLUDE_TEMP_HPP_

/**
 * @brief collection of implementation details that must be removed in close future by replacing with real
 * implementation
 */

#include <terminal.hpp>
#include <types.hpp>
#include <todo.hpp>

TODO_BY_THE_END_OF_MILESTONE0

inline const char *temp_ToStringHex(const u64 x)
{
    static char buf[17];
    buf[16] = '\0';

    for (i64 iter = 60, cur = 0; iter >= 0; iter -= 4)
    {
        const u64 value = (x >> iter) & kBitMask4;
        buf[cur++] = (value < 10) ? ('0' + value) : ('A' + value - 10);
    }

    return buf;
}

inline const char *temp_ToString(u64 x)
{
    static char buf[64];
    static char stack[64];

    u64 cur = 0;
    while (x > 0)
    {
        const u64 value = x % 10;
        stack[cur++]    = '0' + value;

        x /= 10;
    }

    u64 bufCur = 0;
    while (cur > 0)
    {
        buf[bufCur++] = stack[--cur];
    }
    buf[bufCur] = '\0';

    return buf;
}

inline void temp_DisplayHex(const u64 x, const char *msg)
{
    TerminalWriteString(msg);
    TerminalWriteString(": ");
    TerminalWriteString(temp_ToStringHex(x));
    TerminalPutChar('\n');
}

inline void temp_DisplayNum(const u64 x, const char *msg)
{
    TerminalWriteString(msg);
    TerminalWriteString(": ");
    TerminalWriteString(temp_ToString(x));
    TerminalPutChar('\n');
}

#endif // KERNEL_INCLUDE_TEMP_HPP_
