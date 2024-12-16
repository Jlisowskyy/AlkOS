//
// Created by wookie on 12/15/24.
//

#ifndef ALKOS_SERIAL64_H_
#define ALKOS_SERIAL64_H_

#define ALKOS_SERIAL 1

/**
 * @brief initialize the serial port (COM1)
 */
void SerialInit();

/**
 * @brief send a character to the serial port (COM1)
 * @param c the character to send
 */
void SerialPutChar(char c);

/**
 * @brief send a null-terminated string to the serial port (COM1)
 * @param s pointer to the null-terminated string
 */
void SerialPutString(const char *s);

#endif //ALKOS_SERIAL64_H_
