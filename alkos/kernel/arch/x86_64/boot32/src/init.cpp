//
// Created by wookie on 1/2/25.
//
#include <arch_utils.hpp>
#include <terminal.hpp>
#include <tags.hpp>
#include <elf.hpp>
#include <multiboot2.h>
#include <libc.hpp>

// External functions defined in assembly or C
extern "C" int check_multiboot2();
extern "C" int check_cpuid();
extern "C" int check_long_mode();

void setup_page_tables();
extern "C" void enable_paging();
extern "C" void enable_long_mode();
extern "C" void enter_kernel(void* kernel_entry, void* multiboot_info_addr);

char text_buffer[1024];

extern "C" void PreKernelInit(uint32_t boot_loader_magic, void* multiboot_info_addr)
{
    TerminalInit();
    TerminalWriteString(INFO_TAG "In 32-bit mode\n");
    TerminalWriteString(INFO_TAG "Checking for Multiboot2...\n");

    if (boot_loader_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
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

    TerminalWriteString(INFO_TAG "Parsing Multiboot2 tags...\n");
//    uint32_t multiboot_info_size = *(uint32_t*)(multiboot_info_addr);

    multiboot_tag_module* kernel_module = nullptr;
    multiboot_tag* tag;
    for (tag = (multiboot_tag*)(multiboot_info_addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7))) {
        TerminalWriteString(INFO_TAG "Found tag: ");
        TerminalWriteString(GetTagName(tag->type));
        TerminalWriteString("\n");

        if(tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
            multiboot_tag_module* module = (multiboot_tag_module*)tag;
            kernel_module = module;

            // TODO: Modules should be loaded with a specific command line
            // so that the loader knows which module is the kernel
            // but for now, we will just take the last module as the kernel

            TerminalWriteString(INFO_TAG "Module loaded at: ");
            uint32_to_string(module->mod_start, text_buffer);
            TerminalWriteString(text_buffer);
            TerminalWriteString(" - ");
            uint32_to_string(module->mod_end, text_buffer);
            TerminalWriteString(text_buffer);
            TerminalWriteString("\n");
            TerminalWriteString(INFO_TAG "With command line: ");
            if ( (char*)module->cmdline == "\0") {
                TerminalWriteString((char*)module->cmdline);
            } else {
                TerminalWriteString("no command line provided");
            }
            TerminalWriteString("\n");
        }
    }
    TerminalWriteString(SUCCESS_TAG "Multiboot2 tags parsed!\n");

    TerminalWriteString(INFO_TAG "Loading kernel module...\n");
    void* kernel_entry = load_elf64_module((uint8_t*)kernel_module->mod_start, (uint8_t*)kernel_module->mod_end);
    if (kernel_entry == nullptr) {
        TerminalWriteString(ERROR_TAG "Failed to load kernel module!\n");
        OsHangNoInterrupts();
    }
    TerminalWriteString(SUCCESS_TAG "Kernel module loaded!\n");


    enter_kernel(kernel_entry, multiboot_info_addr);
}
