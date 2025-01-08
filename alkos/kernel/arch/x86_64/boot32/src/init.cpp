#include <arch_utils.hpp>
#include <terminal.hpp>
#include <tags.hpp>
#include <elf.hpp>
#include <multiboot2.h>
#include <libc.hpp>
#include <loader_data.hpp>
#include <defines.hpp>

// External functions defined in assembly or C
extern "C" int check_cpuid();
extern "C" int check_long_mode();

extern void SetupPageTables();
extern "C" void enable_paging();
extern "C" void enable_long_mode();
extern "C" void enter_kernel(void* kernel_entry, void* multiboot_info_addr);

// Buffer for text output
char text_buffer[1024];

// External symbols defined in the linker script
extern const char multiboot_header_start[];
extern const char multiboot_header_end[];

extern const char loader_start[];
extern const char loader_end[];

// Data structure that holds information passed from the 32-bit loader to the 64-bit kernel
LoaderData loader_data;

extern "C" void PreKernelInit(uint32_t boot_loader_magic, void* multiboot_info_addr)
{
    TerminalInit();
    TerminalWriteString(INFO_TAG "In 32-bit mode\n");

    ////////////////////////////// Multiboot Check ///////////////////////////////
    TerminalWriteString(INFO_TAG "Checking for Multiboot2...\n");
    if (boot_loader_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        TerminalWriteString("Multiboot2 check failed!\n");
        OsHangNoInterrupts();
    }
    TerminalWriteString(INFO_TAG "Multiboot2 check passed!\n");

    ////////////////////////////// Hardware Checks ///////////////////////////////
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

    //////////////////////// Setting up Paging Structures ////////////////////////
    TerminalWriteString(INFO_TAG "Setting up page tables...\n");
    SetupPageTables();
    TerminalWriteString(SUCCESS_TAG "Page tables setup complete!\n");

    ///////////////////////////// Enabling Hardware //////////////////////////////
    TerminalWriteString(INFO_TAG "Enabling long mode...\n");
    enable_long_mode();
    TerminalWriteString(SUCCESS_TAG "Long mode enabled!\n");

    TerminalWriteString(INFO_TAG "Enabling paging...\n");
    enable_paging();
    TerminalWriteString(SUCCESS_TAG "Paging enabled!\n");

    TerminalWriteString(INFO_TAG "Finished hardware features setup for 32-bit mode.\n");

    /////////////////////// Preparation for jump to 64 bit ///////////////////////
    TerminalWriteString(INFO_TAG "Starting 64-bit kernel...\n");

    TerminalWriteString(INFO_TAG "Parsing Multiboot2 tags...\n");

    ///////////////// Finding Kernel Module in Multiboot Struct //////////////////
    multiboot_tag_module* kernel_module = FindKernelModule(multiboot_info_addr);
    if (kernel_module == nullptr) {
        TerminalWriteString(ERROR_TAG "Failed to find kernel module!\n");
        OsHangNoInterrupts();
    }
    TerminalWriteString(SUCCESS_TAG "Found kernel module in multiboot tags!\n");

    /////////////////////////// Loading Kernel Module ////////////////////////////
    TerminalWriteString(INFO_TAG "Loading kernel module...\n");
    void* kernel_entry = LoadElf64Module((uint8_t*)kernel_module->mod_start, (uint8_t*)kernel_module->mod_end);
    if (kernel_entry == nullptr) {
        TerminalWriteString(ERROR_TAG "Failed to load kernel module!\n");
        OsHangNoInterrupts();
    }
    TerminalWriteString(SUCCESS_TAG "Kernel module loaded!\n");

    ///////////////////// Initializing LoaderData Structure //////////////////////
    loader_data.multiboot_info_addr = (u32)multiboot_info_addr;
    loader_data.multiboot_header_start_addr = (u32)multiboot_header_start;
    loader_data.multiboot_header_end_addr = (u32)multiboot_header_end;
    loader_data.loader_start_addr = (u32)loader_start;
    loader_data.loader_end_addr = (u32)loader_end;

    //////////////////////////// Printing LoaderData Info /////////////////////////
    // TODO: This should properly print the addresses of the loader data structure
    // as %x and not %d
    // TODO: This may print the address range of the kernel module as well
    Uint32ToString((u32)&loader_data, text_buffer);
    TerminalWriteString(INFO_TAG "LoaderData Address: ");
    TerminalWriteString(text_buffer);
    TerminalWriteString("\n");

    Uint32ToString((u32)loader_data.multiboot_info_addr, text_buffer);
    TerminalWriteString(INFO_TAG "LoaderData multiboot_info_addr: ");
    TerminalWriteString(text_buffer);
    TerminalWriteString("\n");

    Uint32ToString((u32)loader_data.multiboot_header_start_addr, text_buffer);
    TerminalWriteString(INFO_TAG "LoaderData multiboot_header_start_addr: ");
    TerminalWriteString(text_buffer);
    TerminalWriteString("\n");

    Uint32ToString((u32)loader_data.multiboot_header_end_addr, text_buffer);
    TerminalWriteString(INFO_TAG "LoaderData multiboot_header_end_addr: ");
    TerminalWriteString(text_buffer);
    TerminalWriteString("\n");

    Uint32ToString((u32)loader_data.loader_start_addr, text_buffer);
    TerminalWriteString(INFO_TAG "LoaderData loader_start_addr: ");
    TerminalWriteString(text_buffer);
    TerminalWriteString("\n");

    Uint32ToString((u32)loader_data.loader_end_addr, text_buffer);
    TerminalWriteString(INFO_TAG "LoaderData loader_end_addr: ");
    TerminalWriteString(text_buffer);
    TerminalWriteString("\n");

    //////////////////////////// Jumping to 64-bit Kernel /////////////////////////
    enter_kernel(kernel_entry, &loader_data);
}
