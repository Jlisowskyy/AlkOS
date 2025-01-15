#include <stdint.h>
#include <kernel_assert.hpp>
#include <paging.hpp>
#include <types.hpp>

// Extern Declarations for Page Tables defined in Assembly

u64 p4_table_loader[512];
u64 p3_table_loader[512];
u64 p2_table_loader[512];
u64 p1_table_loader[512];

u64 p3_table_kernel_identity[512];
u64 p2_table_kernel_identity[512];
u64 p1_table_kernel_identity[512];

u64 p3_table_kernel_higher_half[512];
u64 p2_table_kernel_higher_half[512];
u64 p1_table_kernel_higher_half[512];

// Page Table Entry Flags
static constexpr u32 PRESENT_BIT       = (1 << 0);  // The page is present in memory
static constexpr u32 WRITE_BIT         = (1 << 1);  // The page is writable
static constexpr u32 USER_BIT          = (1 << 2);  // The page is accessible from user mode
static constexpr u32 WRITE_THROUGH_BIT = (1 << 3);  // Writes go directly to memory
static constexpr u32 CACHE_DISABLE_BIT = (1 << 4);  // The page is not cached
static constexpr u32 ACCESSED_BIT      = (1 << 5);  // CPU sets this bit when the page is accessed
static constexpr u32 DIRTY_BIT         = (1 << 6);  // CPU sets this bit when the page is written to
static constexpr u32 HUGE_PAGE_BIT     = (1 << 7);  // Creates a 2 MiB page in PD table
static constexpr u32 GLOBAL_BIT =
    (1 << 8);  // The page isn't flushed from the TLB on address space switch
static constexpr u32 TWO_MIB = 0x200000;
// NO_EXECUTE_BIT is not used in 32-bit PAE

/**
 * @brief Initializes the page tables for PAE paging.
 *
 * This function sets up the page tables to identity map the first 1 GiB of memory
 * using 2 MiB pages. It initializes the Page Map Level 4 (PML4), Page Directory Pointer Table
 * (PDPT), and Page Directory (PD) accordingly.
 */
void IdentityMapFirst4GbOfMemory()
{
    for (int i = 0; i < 512; i++) {
        // Each PD entry maps a 2 MiB page
        MapVirtualMemoryToPhysical(
            TWO_MIB * i, TWO_MIB * i, PRESENT_BIT | WRITE_BIT | HUGE_PAGE_BIT, p3_table_loader,
            p2_table_loader, p1_table_loader
        );
    }
}

/**
 * @brief Identity maps the kernel memory.
 * This is a helper function to allow the elf loader to then move the kernel to the higher half.
 */
void IndentityMapKernelMemory(u64 kernel_start, u64 kernel_end)
{
    for (u64 i = kernel_start; i < kernel_end; i += TWO_MIB) {
        MapVirtualMemoryToPhysical(
            i, i, PRESENT_BIT | WRITE_BIT | HUGE_PAGE_BIT, p3_table_kernel_identity,
            p2_table_kernel_identity, p1_table_kernel_identity
        );
    }
}

/**
 * @brief Maps the kernel memory to the higher half of the virtual address space.
 *
 * @param kernel_start The start of the kernel memory (identity mapped).
 * @param kernel_end The end of the kernel memory (identity mapped).
 *
 * @note This function assumes that the kernel memory is identity mapped.
 */
void MapKernelMemoryToHigherHalf(u64 kernel_start, u64 kernel_end)
{
    R_ASSERT((kernel_start & 0xFFF) == 0);  // Virtual address must be page aligned

    for (u64 i = kernel_start; i < kernel_end; i += TWO_MIB) {
        u64 KernelVirtualAddress  = i - kernel_start + kHigherHalfOffset;
        u64 KernelPhysicalAddress = i;
        MapVirtualMemoryToPhysical(
            KernelVirtualAddress, KernelPhysicalAddress, PRESENT_BIT | WRITE_BIT | HUGE_PAGE_BIT,
            p3_table_kernel_higher_half, p2_table_kernel_higher_half, p1_table_kernel_higher_half
        );
    }
}

/**
 * @brief Maps a virtual address to a physical address using the provided page tables.
 *
 * This function sets up the necessary page table entries to map a given virtual address
 * to a specified physical address with the provided flags.
 * It uses the provided Page Directory Pointer Table (PDPT), Page Directory (PD), and Page Table
 * (PT) And the hardcoded PML4 table to set up the page tables.
 *
 * This is for mapping both the loader memory at it's startup, and the kernel initial memory.
 *
 * @param virtual_address The virtual address to map.
 * @param physical_address The physical address to map to.
 * @param flags The flags to set for the page table entry.
 * @param p3_table The Page Directory Pointer Table to use.
 * @param p2_table The Page Directory to use.
 * @param p1_table The Page Table to use.
 */
void MapVirtualMemoryToPhysical(
    u64 virtual_address, u64 physical_address, u64 flags, u64* p3_table, u64* p2_table,
    u64* p1_table
)
{
    R_ASSERT((virtual_address & 0xFFF) == 0);   // Virtual address must be page aligned
    R_ASSERT((physical_address & 0xFFF) == 0);  // Physical address must be page aligned

    // Calculate the indexes for each level of the page table
    u64 p4_index = (virtual_address >> 39) & 0x1FF;
    u64 p3_index = (virtual_address >> 30) & 0x1FF;
    u64 p2_index = (virtual_address >> 21) & 0x1FF;
    u64 p1_index = (virtual_address >> 12) & 0x1FF;

    // Ensure PML4 entry points to the correct PDPT
    if (!(p4_table_loader[p4_index] & PRESENT_BIT)) {
        p4_table_loader[p4_index] = ((u64)p3_table) | PRESENT_BIT | WRITE_BIT;
    }

    // Ensure PDPT entry points to the correct PD
    if (!(p3_table[p3_index] & PRESENT_BIT)) {
        p3_table[p3_index] = ((u64)p2_table) | PRESENT_BIT | WRITE_BIT;
    }

    if (flags & HUGE_PAGE_BIT) {
        p2_table[p2_index] = physical_address | flags;
        return;
    }

    // Ensure PD entry points to the correct PT
    if (!(p2_table[p2_index] & PRESENT_BIT)) {
        p2_table[p2_index] = ((u64)p1_table) | PRESENT_BIT | WRITE_BIT;
    }

    // Set the PT entry to the physical address with the provided flags
    p1_table[p1_index] = physical_address | flags;
}
