/* internal includes */
#include <string.h>
#include <defines.hpp>
#include <types.hpp>
#include <vga/vga.hpp>

/**
 * @file vga.cpp
 * @brief Implementation of VGA text mode terminal driver
 */

// ------------------------------
// Types and variables
// ------------------------------

/**
 * @name VGA Hardware Text Mode Colors
 * Standard 16-color VGA text mode palette
 * @{
 */
enum VgaColor {
    VGA_COLOR_BLACK         = 0,   ///< Standard black
    VGA_COLOR_BLUE          = 1,   ///< Standard blue
    VGA_COLOR_GREEN         = 2,   ///< Standard green
    VGA_COLOR_CYAN          = 3,   ///< Standard cyan
    VGA_COLOR_RED           = 4,   ///< Standard red
    VGA_COLOR_MAGENTA       = 5,   ///< Standard magenta
    VGA_COLOR_BROWN         = 6,   ///< Standard brown
    VGA_COLOR_LIGHT_GREY    = 7,   ///< Standard light grey
    VGA_COLOR_DARK_GREY     = 8,   ///< Standard dark grey
    VGA_COLOR_LIGHT_BLUE    = 9,   ///< Bright blue
    VGA_COLOR_LIGHT_GREEN   = 10,  ///< Bright green
    VGA_COLOR_LIGHT_CYAN    = 11,  ///< Bright cyan
    VGA_COLOR_LIGHT_RED     = 12,  ///< Bright red
    VGA_COLOR_LIGHT_MAGENTA = 13,  ///< Bright magenta
    VGA_COLOR_LIGHT_BROWN   = 14,  ///< Yellow (bright brown)
    VGA_COLOR_WHITE         = 15,  ///< Bright white
};
/** @} */

/**
 * @name VGA Display Constants
 * Standard VGA text mode dimensions
 * @{
 */
static constexpr size_t kVgaWidth  = 80;  ///< Standard VGA text mode width
static constexpr size_t kVgaHeight = 25;  ///< Standard VGA text mode height
/** @} */

/**
 * @name Terminal State Variables
 * Global state tracking for the terminal
 * @{
 */
static size_t g_TerminalRow;        ///< Current cursor row position
static size_t g_TerminalColumn;     ///< Current cursor column position
static uint8_t g_TerminalColor;     ///< Current text color attributes
static uint16_t *g_TerminalBuffer;  ///< Pointer to VGA memory buffer (0xB8000)
/** @} */

/**
 * @brief Create a VGA color attribute byte
 *
 * @param fg Foreground color
 * @param bg Background color
 * @return Combined color attribute byte
 *
 * Combines foreground and background colors into a single byte
 * where the background color is in the high nibble.
 */
static FORCE_INLINE uint8_t VgaEntryColor(const VgaColor fg, const VgaColor bg)
{
    return fg | bg << 4;
}

/**
 * @brief Create a VGA character entry
 *
 * @param uc Character to display
 * @param color Color attribute byte
 * @return Combined VGA character entry
 *
 * Creates a 16-bit VGA character entry with the character in the low byte
 * and the color attribute in the high byte.
 */
static FORCE_INLINE uint16_t VgaEntry(const unsigned char uc, const uint8_t color)
{
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

void VgaTerminalInit()
{
    g_TerminalRow    = 0;
    g_TerminalColumn = 0;
    g_TerminalColor  = VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    g_TerminalBuffer = reinterpret_cast<uint16_t *>(0xB8000);

    for (size_t y = 0; y < kVgaHeight; y++) {
        for (size_t x = 0; x < kVgaWidth; x++) {
            const size_t index      = y * kVgaWidth + x;
            g_TerminalBuffer[index] = VgaEntry(' ', g_TerminalColor);
        }
    }
}

void VgaTerminalSetColor(const uint8_t color) { g_TerminalColor = color; }

void VgaTerminalPutEntryAt(const char c, const uint8_t color, const size_t x, const size_t y)
{
    const size_t index      = y * kVgaWidth + x;
    g_TerminalBuffer[index] = VgaEntry(c, color);
}

void VgaTerminalPutChar(const char c)
{
    VgaTerminalPutEntryAt(c, g_TerminalColor, g_TerminalColumn, g_TerminalRow);
    if (++g_TerminalColumn == kVgaWidth) {
        g_TerminalColumn = 0;
        if (++g_TerminalRow == kVgaHeight)
            g_TerminalRow = 0;
    }
}

void VgaTerminalWrite(const char *data, const size_t size)
{
    for (size_t i = 0; i < size; i++) VgaTerminalPutChar(data[i]);
}

void VgaTerminalWriteString(const char *data)
{
    VgaTerminalSetColor(VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    VgaTerminalWrite(data, strlen(data));
}

void VgaTerminalWriteError(const char *data)
{
    VgaTerminalSetColor(VgaEntryColor(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    VgaTerminalWrite(data, strlen(data));
    VgaTerminalSetColor(VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}
