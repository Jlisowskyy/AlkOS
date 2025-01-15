#ifndef ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_SETUP_PAGE_TABLES_HPP_
#define ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_SETUP_PAGE_TABLES_HPP_

#include <types.hpp>

/**
 * In x86_64 only 48 bits of an address are free to use, the other 16 bits are copies of the most
 * significant bit. So the higher half of the virtual address space is from 0xFFFF800000000000 to
 * 0xFFFFFFFFFFFFFFFF.
 */
static constexpr u64 kHigherHalfOffset = 0xFFFF800000000000;

void ClearPageTables();
void IdentityMapFirst4GbOfMemory();
void IndentityMapKernelMemory(u64 kernel_start, u64 kernel_end);
void MapKernelMemoryToHigherHalf(u64 kernel_start, u64 kernel_end);
void MapVirtualMemoryToPhysical(
    u32 virtual_address_lower, u32 virtual_address_upper, u32 physical_address_lower,
    u32 physical_address_upper, u32 flags, u64* p4_table, u64* p3_table, u64* p2_table,
    u64* p1_table
);

#endif  // ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_SETUP_PAGE_TABLES_HPP_
