#include <multiboot2.h>
#include <arch_utils.hpp>
#include <debug.hpp>
#include <defines.hpp>
#include <elf.hpp>
#include <loader_data.hpp>
#include <multiboot2_extensions.hpp>
#include <paging.hpp>
#include <terminal.hpp>

// External functions defined in assembly or C
extern "C" int check_cpuid();
extern "C" int check_long_mode();

extern "C" void enable_paging();
extern "C" void enable_long_mode();
extern "C" void enter_kernel(
    void* higher_32_bits_of_kernel_entry_address, void* lower_32_bits_of_kernel_entry_address,
    void* loader_data_address
);

// Buffer for text output
char text_buffer[1024];

// External symbols defined in the linker script
extern const char multiboot_header_start[];
extern const char multiboot_header_end[];

extern const char loader_start[];
extern const char loader_end[];

// Data structure that holds information passed from the 32-bit loader to the 64-bit kernel
LoaderData loader_data;

// Helper
static constexpr u64 k32BitMask = 0x00000000FFFFFFFF;

extern "C" void PreKernelInit(uint32_t boot_loader_magic, void* multiboot_info_addr)
{
    TerminalInit();
    TRACE_INFO("In 32 bit mode");

    ////////////////////////////// Multiboot Check ///////////////////////////////
    TRACE_INFO("Checking for Multiboot2...");
    if (boot_loader_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        KernelPanic("Multiboot2 check failed!");
    }
    TRACE_SUCCESS("Multiboot2 check passed!");

    ////////////////////////////// Hardware Checks ///////////////////////////////
    TRACE_INFO("Starting pre-kernel initialization");
    BlockHardwareInterrupts();

    TRACE_INFO("Checking for hardware features");

    TRACE_INFO("Checking for CPUID...");
    if (check_cpuid()) {
        KernelPanic("CPUID check failed!");
    }
    TRACE_SUCCESS("CPUID check passed!");

    TRACE_INFO("Checking for long mode...");
    if (check_long_mode()) {
        KernelPanic("Long mode check failed!");
    }
    TRACE_SUCCESS("Long mode check passed!");

    TRACE_INFO("Enabling hardware features...");

    //////////////////////// Setting up Paging Structures ////////////////////////
    TRACE_INFO("Identity mapping first 4 GiB of memory...");
    IdentityMapFirst4GbOfMemory();
    TRACE_SUCCESS("Identity mapping complete!");

    ///////////////////////////// Enabling Hardware //////////////////////////////
    TRACE_INFO("Enabling long mode...");
    enable_long_mode();
    TRACE_SUCCESS("Long mode enabled!");

    TRACE_INFO("Enabling paging...");
    enable_paging();
    TRACE_SUCCESS("Paging enabled!");

    TRACE_INFO("Finished hardware features setup for 32-bit mode.");

    /////////////////////// Preparation for jump to 64 bit ///////////////////////
    TRACE_INFO("Starting 64-bit kernel...");

    TRACE_INFO("Parsing Multiboot2 tags...");

    ///////////////// Finding Kernel Module in Multiboot Struct //////////////////
    multiboot_tag_module* kernel_module = FindKernelModule(multiboot_info_addr);
    if (kernel_module == nullptr) {
        KernelPanic("Kernel module not found in multiboot tags!");
    }
    TRACE_SUCCESS("Found kernel module in multiboot tags!");

    byte* kernel_module_start = reinterpret_cast<byte*>(kernel_module->mod_start);
    byte* kernel_module_end   = reinterpret_cast<byte*>(kernel_module->mod_end);

    TRACE_INFO("Kernel module start: 0x%X", kernel_module_start);
    TRACE_INFO("Kernel module end: 0x%X", kernel_module_end);
    TRACE_INFO("Identity mapping %x-%x", kernel_module_start, kernel_module_end);
    // Cast to u64 with first 32 bits zeroed
    u64 kernel_module_start_u64 = reinterpret_cast<u64>(kernel_module_start) & k32BitMask;
    u64 kernel_module_end_u64   = reinterpret_cast<u64>(kernel_module_end) & k32BitMask;
    // TODO: Just name it temp identity map since it doesn't and won't map kernel binary code
    // This usage her maps kernel elf file, not the kernel itself
    IndentityMapKernelMemory(kernel_module_start_u64, kernel_module_end_u64);

    /////////////////////////// Loading Kernel Module ////////////////////////////
    TRACE_INFO("Loading kernel module...");
    void* kernel_entry_relative_to_elf_start =
        // TODO rework the loading to load the kernel to some cool physical address
        LoadElf64Module(kernel_module_start, kernel_module_end);
    if (kernel_entry_relative_to_elf_start == nullptr) {
        KernelPanic("Failed to load kernel module!");
    }
    TRACE_SUCCESS("Kernel module loaded!");

    // TODO: Map the higher half of virtual memory to the cool physical address of kernel executable
    // This maps the elf file, which is not the exec, this is my mistake
    MapKernelMemoryToHigherHalf(kernel_module_start_u64, kernel_module_end_u64);

    ///////////////////// Initializing LoaderData Structure //////////////////////
    loader_data.multiboot_info_addr         = (u32)multiboot_info_addr;
    loader_data.multiboot_header_start_addr = (u32)multiboot_header_start;
    loader_data.multiboot_header_end_addr   = (u32)multiboot_header_end;
    loader_data.loader_start_addr           = (u32)loader_start;
    loader_data.loader_end_addr             = (u32)loader_end;

    //////////////////////////// Printing LoaderData Info /////////////////////////
    // Convert addresses to hexadecimal strings
    TRACE_INFO("LoaderData Address: 0x%X", (u32)&loader_data);
    TRACE_INFO("LoaderData multiboot_info_addr: 0x%X", loader_data.multiboot_info_addr);
    TRACE_INFO(
        "LoaderData multiboot_header_start_addr: 0x%X", loader_data.multiboot_header_start_addr
    );
    TRACE_INFO("LoaderData multiboot_header_end_addr: 0x%X", loader_data.multiboot_header_end_addr);
    TRACE_INFO("LoaderData loader_start_addr: 0x%X", loader_data.loader_start_addr);
    TRACE_INFO("LoaderData loader_end_addr: 0x%X", loader_data.loader_end_addr);

    //////////////////////////// Jumping to 64-bit Kernel /////////////////////////
    TRACE_INFO("Jumping to 64-bit kernel...");
    enter_kernel(
        reinterpret_cast<void*>(kHigherHalfOffset >> 32), kernel_entry_relative_to_elf_start,
        &loader_data
    );
}
