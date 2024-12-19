#ifndef KERNEL_INCLUDE_TERMINA_HPP_
#define KERNEL_INCLUDE_TERMINA_HPP_

extern "C" {
    void TerminalInit();

    void TerminalPutChar(char c);

    void TerminalWriteString(const char *data);

    void TerminalWriteError(const char *data);
}

#endif // KERNEL_INCLUDE_TERMINA_HPP_
