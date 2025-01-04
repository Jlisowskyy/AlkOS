//
// Created by wookie on 1/4/25.
//

#include <stdint.h>

// Extern Declarations for Page Tables defined in Assembly
extern uint64_t p4_table[];
extern uint64_t p3_table[];
extern uint64_t p2_table[];
extern uint64_t p1_table[];

// Page Table Entry Flags
#define PRESENT_BIT       (1 << 0)  // The page is present in memory
#define WRITE_BIT         (1 << 1)  // The page is writable
#define USER_BIT          (1 << 2)  // The page is accessible from user mode
#define WRITE_THROUGH_BIT (1 << 3)  // Writes go directly to memory
#define CACHE_DISABLE_BIT (1 << 4)  // The page is not cached
#define ACCESSED_BIT      (1 << 5)  // CPU sets this bit when the page is accessed
#define DIRTY_BIT         (1 << 6)  // CPU sets this bit when the page is written to
#define HUGE_PAGE_BIT     (1 << 7)  // Creates a 2 MiB page in PD table
#define GLOBAL_BIT        (1 << 8)  // The page isn't flushed from the TLB on address space switch
// NO_EXECUTE_BIT is not used in 32-bit PAE

/**
 * @brief Initializes the page tables for PAE paging.
 *
 * This function sets up the page tables to identity map the first 1 GiB of memory
 * using 2 MiB pages. It initializes the Page Map Level 4 (PML4), Page Directory Pointer Table
 * (PDPT), and Page Directory (PD) accordingly.
 *
 * @return int 0 because the function always succeeds
 */
int setup_page_tables()
{
    // Initialize the PML4 entry to point to the PDPT with present and writable flags
    p4_table[0] = ((uint64_t)p3_table) | PRESENT_BIT | WRITE_BIT;

    // Initialize the PDPT entry to point to the PD with present and writable flags
    p3_table[0] = ((uint64_t)p2_table) | PRESENT_BIT | WRITE_BIT;

    // Identity map the first 1 GiB of memory using 2 MiB pages
    for (int i = 0; i < 512; i++) {
        // Each PD entry maps a 2 MiB page
        p2_table[i] = ((uint64_t)(0x200000 * i)) | PRESENT_BIT | WRITE_BIT | HUGE_PAGE_BIT;
    }

    return 0;
}