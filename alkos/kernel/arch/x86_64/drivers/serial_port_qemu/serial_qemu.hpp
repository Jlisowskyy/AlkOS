#ifndef KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_

#include <types.hpp>

/**
 * @file serial_qemu.hpp
 * @brief Interface for QEMU serial port communication
 *
 * This header provides the interface for initializing and using a serial port
 * for communication with QEMU's terminal. It supports basic I/O operations
 * including character and string transmission, as well as line-based reading.
 */

extern "C" {

/**
 * @brief Initialize the QEMU serial port terminal
 *
 * Configures COM1 port with the following settings:
 * - Baud rate: 38400
 * - Data bits: 8
 * - Stop bits: 1
 * - No parity
 * - FIFO enabled
 * Performs a loopback test to verify the configuration.
 */
void QemuTerminalInit();

/**
 * @brief Send a single character through the serial port
 *
 * @param c The character to transmit
 *
 * Blocks until the transmission line is empty before sending.
 */
void QemuTerminalPutChar(char c);

/**
 * @brief Send a null-terminated string through the serial port
 *
 * @param s Pointer to the null-terminated string to transmit
 *
 * Transmits each character in the string sequentially,
 * blocking until each character is sent.
 */
void QemuTerminalWriteString(const char *s);

/**
 * @brief Receive a single character from the serial port
 *
 * @return The received character
 *
 * Blocks until a character is received from the serial port.
 */
char QemuTerminalGetChar();

/**
 * @brief Read a line of text from the serial port
 *
 * @param buffer Destination buffer for the received line
 * @param size Maximum number of characters to read (including null terminator)
 * @return Number of characters read (including null terminator)
 *
 * Reads characters until either a carriage return ('\r', used by qemu) is received
 * or the buffer is full. The resulting string is null-terminated.
 * The returned size includes the null terminator.
 */
size_t QemuTerminalReadLine(char *buffer, size_t size);

}

#endif  // KERNEL_ARCH_X86_64_DRIVERS_SERIAL_PORT_QEMU_SERIAL_QEMU_HPP_
