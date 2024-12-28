#ifndef KERNEL_INCLUDE_TERMINA_HPP_
#define KERNEL_INCLUDE_TERMINA_HPP_

#include <todo.hpp>

TODO_BY_THE_END_OF_MILESTONE0
/* REWORK THAT */

extern "C"
{
    void TerminalInit();

    void TerminalPutChar(char c);

    void TerminalWriteString(const char *data);

    void TerminalWriteError(const char *data);
}

#endif // KERNEL_INCLUDE_TERMINA_HPP_
