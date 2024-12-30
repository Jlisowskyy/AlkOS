#ifndef KERNEL_ABI_TERMINA_HPP_
#define KERNEL_ABI_TERMINA_HPP_

#include <todo.hpp>
#include <types.hpp>

TODO_BY_THE_END_OF_MILESTONE0
/* REWORK THAT */

extern "C" {
void TerminalInit();

void TerminalPutChar(char c);

void TerminalWriteString(const char *data);

void TerminalWriteError(const char *data);

char TerminalGetChar();

size_t TerminalReadLine(char *buffer, size_t size);
}

#endif  // KERNEL_ABI_TERMINA_HPP_
