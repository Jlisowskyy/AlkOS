#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__x86_64__)
#error "AlkOS needs to be compiled with a x86_64-elf compiler"
#endif

/* internal includes */
#include <multiboot2.h>
#include <arch_utils.hpp>
#include <debug.hpp>
#include <drivers/pic8259/pic8259.hpp>
#include <init.hpp>
#include <interrupts/idt.hpp>
#include <loader_data.hpp>
#include <multiboot2_extensions.hpp>
#include <physical_memory_manager.hpp>
#include <terminal.hpp>

/* external init procedures */
extern "C" void enable_osxsave();

extern "C" void enable_sse();

extern "C" void enable_avx();

extern "C" void PreKernelInit(LoaderData *loader_data)
{
    TerminalInit();
    TRACE_INFO("In 64 bit mode");

    TRACE_INFO("Checking for LoaderData...");
    TRACE_INFO("LoaderData Address: 0x%X", loader_data);
    if (loader_data == nullptr) {
        KernelPanic("LoaderData not found!");
    }
    TRACE_INFO("LoaderData multiboot_info_addr: 0x%X", loader_data->multiboot_info_addr);
    TRACE_INFO(
        "LoaderData multiboot_header_start_addr: 0x%X", loader_data->multiboot_header_start_addr
    );
    TRACE_INFO(
        "LoaderData multiboot_header_end_addr: 0x%X", loader_data->multiboot_header_end_addr
    );
    TRACE_INFO("LoaderData loader_start_addr: 0x%X", loader_data->loader_start_addr);
    TRACE_INFO("LoaderData loader_end_addr: 0x%X", loader_data->loader_end_addr);

    TRACE_INFO("Starting pre-kernel initialization");

    TRACE_INFO("Starting to setup CPU features");
    BlockHardwareInterrupts();

    /* NOTE: sequence is important */
    TRACE_INFO("Setting up OS XSAVE...");
    enable_osxsave();
    TRACE_SUCCESS("OS XSAVE setup complete!");

    TRACE_INFO("Setting up SSE...");
    enable_sse();
    TRACE_SUCCESS("SSE setup complete!");

    TRACE_INFO("Setting up AVX...");
    enable_avx();
    TRACE_SUCCESS("AVX setup complete!");

    TRACE_INFO("Setting up PIC units...");
    InitPic8259(kIrq1Offset, kIrq2Offset);
    TRACE_SUCCESS("PIC units setup complete!");

    TRACE_INFO("Setting up IDT...");
    IdtInit();
    TRACE_SUCCESS("IDT setup complete!");

    EnableHardwareInterrupts();
    TRACE_INFO("Finished cpu features setup.");

    TRACE_INFO("Mapping physical memory...");
    multiboot_tag_mmap *mmap_tag =
        FindMemoryMap(reinterpret_cast<void *>(loader_data->multiboot_info_addr));
    if (mmap_tag == nullptr) {
        KernelPanic("Memory map tag not found!");
    }
    TRACE_INFO("Memory map found at: 0x%X", mmap_tag);
    TRACE_INFO("Memory map size: %d", mmap_tag->size);
    TRACE_INFO("Memory map entry size: %d", mmap_tag->entry_size);
    TRACE_INFO("Memory map entry version: %d", mmap_tag->entry_version);

    // TODO: Run a first pass here, just calculating the lowest and biggest avaliable
    // physical address, then map all physical memory to some predefined virtual region
    // of length biggest-lowest. This will allow to operate on physical memory by just adding
    // an offset to a virtual address. Then mapping and unmapping physical memory
    // based on the map below will actually work
    u64 total_memory_size = 0;
    for (multiboot_memory_map_t *mmap = mmap_tag->entries;
         reinterpret_cast<multiboot_uint8_t *>(mmap) <
         reinterpret_cast<multiboot_uint8_t *>(mmap_tag) + mmap_tag->size;
         mmap = reinterpret_cast<multiboot_memory_map_t *>(
             reinterpret_cast<unsigned long>(mmap) + mmap_tag->entry_size
         )) {
        TRACE_INFO(
            "Memory map entry: base_addr = 0x%x%x,"
            " length = 0x%x%x, type = 0x%x",
            static_cast<u32>(mmap->addr >> 32), static_cast<u32>(mmap->addr & 0xffffffff),
            static_cast<u32>(mmap->len >> 32), static_cast<u32>(mmap->len & 0xffffffff),
            static_cast<u64>(mmap->type)
        );
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            TRACE_INFO(
                "Adding memory region %x-%x to available memory", mmap->addr, mmap->addr + mmap->len
            );
            // TODO: Add memory region to available memory
            // PhysicalMemoryManager::GetInstance().FreeRange(
            //     reinterpret_cast<byte *>(mmap->addr),
            //     reinterpret_cast<byte *>(mmap->addr + mmap->len)
            // );
            total_memory_size += mmap->len;
        } else {
            TRACE_INFO("Memory reserved");
        }
    }
    TRACE_INFO("Total memory size: %d MB", total_memory_size / 1024 / 1024);

    TRACE_INFO("Pre-kernel initialization finished.");
}
