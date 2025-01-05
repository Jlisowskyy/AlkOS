//
// Created by wookie on 1/2/25.
//
#include <arch_utils.hpp>
#include <terminal.hpp>
#include <tags.hpp>

// External functions defined in assembly or C
extern "C" int check_multiboot2();
extern "C" int check_cpuid();
extern "C" int check_long_mode();

void setup_page_tables();
extern "C" void enable_paging();
extern "C" void enable_long_mode();

extern "C" void PreKernelInit()
{
    asm volatile("push %eax");  // Multiboot2 magic value is in "eax"

    TerminalInit();
    TerminalWriteString(INFO_TAG "In 32-bit mode\n");
    TerminalWriteString(INFO_TAG "Checking for Multiboot2...\n");

    asm volatile("pop %eax");
    if (check_multiboot2()) {
        TerminalWriteString("Multiboot2 check failed!\n");
        OsHangNoInterrupts();
    }
    TerminalWriteString(INFO_TAG "Multiboot2 check passed!\n");

    TerminalWriteString(INFO_TAG "Starting pre-kernel initialization\n");
    BlockHardwareInterrupts();

    TerminalWriteString(INFO_TAG "Checking for hardware features\n");

    TerminalWriteString(INFO_TAG "Checking for CPUID...\n");
    if (check_cpuid()) {
        TerminalWriteString(ERROR_TAG "CPUID check failed!\n");
        OsHangNoInterrupts();
    }
    TerminalWriteString(SUCCESS_TAG "CPUID check passed!\n");

    TerminalWriteString(INFO_TAG "Checking for long mode...\n");
    if (check_long_mode()) {
        TerminalWriteString(ERROR_TAG "Long mode check failed!\n");
        OsHangNoInterrupts();
    }
    TerminalWriteString(SUCCESS_TAG "Long mode check passed!\n");

    TerminalWriteString(INFO_TAG "Enabling hardware features...\n");

    TerminalWriteString(INFO_TAG "Setting up page tables...\n");
    setup_page_tables();
    TerminalWriteString(SUCCESS_TAG "Page tables setup complete!\n");

    TerminalWriteString(INFO_TAG "Enabling long mode...\n");
    enable_long_mode();
    TerminalWriteString(SUCCESS_TAG "Long mode enabled!\n");

    TerminalWriteString(INFO_TAG "Enabling paging...\n");
    enable_paging();
    TerminalWriteString(SUCCESS_TAG "Paging enabled!\n");

    TerminalWriteString(INFO_TAG "Finished hardware features setup for 32-bit mode.\n");

    TerminalWriteString(INFO_TAG "Starting 64-bit kernel...\n");
    OsHangNoInterrupts();
}
