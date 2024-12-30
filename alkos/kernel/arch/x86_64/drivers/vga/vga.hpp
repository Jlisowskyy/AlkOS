#ifndef KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_

#include <types.hpp>

/**
 * @file vga.hpp
 * @brief Interface for VGA text mode terminal driver
 *
 * This header provides the interface for initializing and controlling a VGA text mode
 * terminal. It supports basic text output operations with color control in a
 * standard 80x25 text mode display.
 */

extern "C" {

/**
 * @brief Initialize the VGA text mode terminal
 *
 * Sets up a 80x25 text mode display with light grey text on black background.
 * Clears the screen and positions the cursor at (0,0).
 */
void VgaTerminalInit();

/**
 * @brief Set the current text color
 *
 * @param color Combined foreground and background color value
 *              (foreground in lower 4 bits, background in upper 4 bits)
 */
void VgaTerminalSetColor(uint8_t color);

/**
 * @brief Write a character with specified color at given coordinates
 *
 * @param c Character to display
 * @param color Color attribute for the character
 * @param x X-coordinate (0 to 79)
 * @param y Y-coordinate (0 to 24)
 */
void VgaTerminalPutEntryAt(char c, uint8_t color, size_t x, size_t y);

/**
 * @brief Write a character at the current cursor position
 *
 * @param c Character to display
 *
 * Automatically advances the cursor and handles wraparound
 * when reaching the end of a line or screen.
 */
void VgaTerminalPutChar(char c);

/**
 * @brief Write a string of specified length
 *
 * @param data Pointer to the string data
 * @param size Number of characters to write
 */
void VgaTerminalWrite(const char *data, size_t size);

/**
 * @brief Write a null-terminated string in default color (light grey on black)
 *
 * @param data Pointer to the null-terminated string
 */
void VgaTerminalWriteString(const char *data);

/**
 * @brief Write a null-terminated string in error color (light red on black)
 *
 * @param data Pointer to the null-terminated string
 *
 * Temporarily changes color to light red, writes the string,
 * then restores the previous color.
 */
void VgaTerminalWriteError(const char *data);
}

#endif  // KERNEL_ARCH_X86_64_DRIVERS_VGA_VGA_HPP_
