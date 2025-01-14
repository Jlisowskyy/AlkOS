#include <kernel_assert.hpp>
#include <physical_mem_allocator.hpp>
#include "memory.h"

PhysicalMemAllocator::PhysicalMemAllocator() : free_pages_(nullptr) {}

PhysicalMemAllocator::~PhysicalMemAllocator()
{
    // There should be no need to clean up the free_pages_ list since the kernel is not supposed to
    // be shut down
}

void PhysicalMemAllocator::FreeRange(byte *start, byte *end)
{
    byte *current = start;
    current =
        reinterpret_cast<byte *>(AlignAddressToPhysicalPageSize(reinterpret_cast<u64>(current)));
    while (current < end) {
        FreePage(current);
        current += kPhysicalPageSize;
    }
}

byte *PhysicalMemAllocator::AllocatePage()
{
    if (free_pages_ == nullptr) {
        return nullptr;
    }

    PhysicalMemList *page = free_pages_;
    free_pages_           = free_pages_->next;
    return reinterpret_cast<byte *>(page);
}

void PhysicalMemAllocator::FreePage(byte *page)
{
    R_ASSERT(page != nullptr);
    R_ASSERT(reinterpret_cast<u64>(page) % kPhysicalPageSize == 0);

    memset(page, 1, kPhysicalPageSize);  // Fill with 1s to catch use-after-free bugs

    auto *new_page = reinterpret_cast<PhysicalMemList *>(page);
    new_page->next = free_pages_;
    free_pages_    = new_page;
}
