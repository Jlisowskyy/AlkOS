//
// Created by wookie on 12/16/24.
//

#include "serial64.h"

extern "C" {
    void serial_init();
    void serial_putchar(char c);
    void serial_puts(const char *s);
}

void SerialInit() {
    serial_init();
}

void SerialPutChar(char c) {
    serial_putchar(c);
}

void SerialPutString(const char *s) {
    serial_puts(s);
}
