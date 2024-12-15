//
// Created by wookie on 12/15/24.
//

#ifndef ALKOS_SERIAL64_H_
#define ALKOS_SERIAL64_H_

/**
 * @brief initialize the serial port (COM1)
 */
extern "C" void serial_init();

/**
 * @brief send a character to the serial port (COM1)
 * @param c the character to send
 */
extern "C" void serial_putchar(char c);

/**
 * @brief send a null-terminated string to the serial port (COM1)
 * @param s pointer to the null-terminated string
 */
extern "C" void serial_puts(const char *s);

#endif //ALKOS_SERIAL64_H_
