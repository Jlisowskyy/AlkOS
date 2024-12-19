#include <vga/vga.hpp>

/* internal includes */
#include <comp.h>

static constexpr uint32_t VGA_WIDTH = 80;
static constexpr uint32_t VGA_HEIGHT = 25;

static uint32_t g_TerminalRow;
static uint32_t g_TerminalColumn;
static uint8_t g_TerminalColor;
static uint16_t *g_TerminalBuffer;

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static uint8_t VgaEntryColor(const vga_color fg, const vga_color bg) {
    return fg | bg << 4;
}

static uint16_t VgaEntry(const unsigned char uc, const uint8_t color) {
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

static uint32_t Strlen(const char *str) {
    uint32_t len = 0;

    while (str[len]) {
        len++;
    }

    return len;
}

DEF_MULTITARGET_FUNC(void, VgaTerminalInit, ()) {
    g_TerminalRow = 0;
    g_TerminalColumn = 0;
    g_TerminalColor = VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    g_TerminalBuffer = (uint16_t *) 0xB8000;
    for (uint32_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint32_t x = 0; x < VGA_WIDTH; x++) {
            const uint32_t index = y * VGA_WIDTH + x;
            g_TerminalBuffer[index] = VgaEntry(' ', g_TerminalColor);
        }
    }
}

DEF_MULTITARGET_FUNC(void, VgaTerminalSetColor, (const uint8_t color)) {
    g_TerminalColor = color;
}

DEF_MULTITARGET_FUNC(void, VgaTerminalPutEntryAt,
                     (const char c, const uint8_t color, const uint32_t x,const uint32_t y)) {
    const size_t index = y * VGA_WIDTH + x;
    g_TerminalBuffer[index] = VgaEntry(c, color);
}

DEF_MULTITARGET_FUNC(void, VgaTerminalPutChar, (const char c)) {
    MULTITARGET_FUNC(VgaTerminalPutEntryAt)(c, g_TerminalColor, g_TerminalColumn, g_TerminalRow);
    if (++g_TerminalColumn == VGA_WIDTH) {
        g_TerminalColumn = 0;

        if (++g_TerminalRow == VGA_HEIGHT) {
            g_TerminalRow = 0;
        }
    }
}

DEF_MULTITARGET_FUNC(void, VgaTerminalWrite, (const char *data, const uint32_t size)) {
    for (uint32_t i = 0; i < size; i++) {
        MULTITARGET_FUNC(VgaTerminalPutChar)(data[i]);
    }
}

DEF_MULTITARGET_FUNC(void, VgaTerminalWriteString, (const char *data)) {
    MULTITARGET_FUNC(VgaTerminalSetColor)(VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    MULTITARGET_FUNC(VgaTerminalWrite)(data, Strlen(data));
}

DEF_MULTITARGET_FUNC(void, VgaTerminalWriteError, (const char *data)) {
    MULTITARGET_FUNC(VgaTerminalSetColor)(VgaEntryColor(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    MULTITARGET_FUNC(VgaTerminalWrite)(data, Strlen(data));
    MULTITARGET_FUNC(VgaTerminalSetColor)(VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}
