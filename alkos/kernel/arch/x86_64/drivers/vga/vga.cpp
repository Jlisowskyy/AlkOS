/* internal includes */
#include <vga/vga.hpp>
#include <types.hpp>
#include <defines.hpp>

// ------------------------------
// Types and variables
// ------------------------------

/* Hardware text mode color constants. */
enum VgaColor {
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

static constexpr size_t kVgaWidth = 80;
static constexpr size_t kVgaHeight = 25;

static size_t g_TerminalRow;
static size_t g_TerminalColumn;
static uint8_t g_TerminalColor;
static uint16_t *g_TerminalBuffer;

static FORCE_INLINE uint8_t VgaEntryColor(const VgaColor fg, const VgaColor bg) {
    return fg | bg << 4;
}

static FORCE_INLINE uint16_t VgaEntry(const unsigned char uc, const uint8_t color) {
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

static size_t Strlen(const char *str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void VgaTerminalInit() {
    g_TerminalRow = 0;
    g_TerminalColumn = 0;
    g_TerminalColor = VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    g_TerminalBuffer = reinterpret_cast<uint16_t *>(0xB8000);

    for (size_t y = 0; y < kVgaHeight; y++) {
        for (size_t x = 0; x < kVgaWidth; x++) {
            const size_t index = y * kVgaWidth + x;
            g_TerminalBuffer[index] = VgaEntry(' ', g_TerminalColor);
        }
    }
}

void VgaTerminalSetColor(const uint8_t color) {
    g_TerminalColor = color;
}

void VgaTerminalPutEntryAt(const char c, const uint8_t color, const size_t x, const size_t y) {
    const size_t index = y * kVgaWidth + x;
    g_TerminalBuffer[index] = VgaEntry(c, color);
}

void VgaTerminalPutChar(const char c) {
    VgaTerminalPutEntryAt(c, g_TerminalColor, g_TerminalColumn, g_TerminalRow);
    if (++g_TerminalColumn == kVgaWidth) {
        g_TerminalColumn = 0;
        if (++g_TerminalRow == kVgaHeight)
            g_TerminalRow = 0;
    }
}

void VgaTerminalWrite(const char *data, const size_t size) {
    for (size_t i = 0; i < size; i++)
        VgaTerminalPutChar(data[i]);
}

void VgaTerminalWriteString(const char *data) {
    VgaTerminalSetColor(VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    VgaTerminalWrite(data, Strlen(data));
}

void VgaTerminalWriteError(const char *data) {
    VgaTerminalSetColor(VgaEntryColor(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    VgaTerminalWrite(data, Strlen(data));
    VgaTerminalSetColor(VgaEntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}
