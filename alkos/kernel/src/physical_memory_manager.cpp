#include <assert.h>
#include <memory.h>
#include <debug.hpp>
#include <physical_memory_manager.hpp>

TODO_WHEN_MULTITHREADING

PhysicalMemoryManager instance{};

PhysicalMemoryManager::PhysicalMemoryManager() : free_pages_(nullptr) {}

PhysicalMemoryManager::~PhysicalMemoryManager()
{
    // There should be no need to clean up the free_pages_ list since the kernel is not supposed to
    // be shut down
}

PhysicalMemoryManager &PhysicalMemoryManager::GetInstance() { return instance; }

void PhysicalMemoryManager::FreeRange(byte *virtual_start_address, const byte *virtual_end_address)
{
    byte *current = virtual_start_address;
    current =
        reinterpret_cast<byte *>(AlignAddressToPhysicalPageSize(reinterpret_cast<u64>(current)));
    // Allocate only whole pages, never half a page
    while (current + kPhysicalPageSize - 1 < virtual_end_address) {
        FreePage(current);
        current += kPhysicalPageSize;
    }
}

byte *PhysicalMemoryManager::AllocatePage()
{
    if (free_pages_ == nullptr) {
        return nullptr;
    }

    PhysicalMemList *page = free_pages_;
    free_pages_           = free_pages_->next;
    return reinterpret_cast<byte *>(page);
}

void PhysicalMemoryManager::FreePage(byte *virtual_page_address)
{
    R_ASSERT(virtual_page_address != nullptr);
    R_ASSERT(reinterpret_cast<u64>(virtual_page_address) % kPhysicalPageSize == 0);
    TRACE_INFO("Freeing page: 0x%X", virtual_page_address);

    memset(
        virtual_page_address, 1, kPhysicalPageSize
    );  // Fill with 1s to catch use-after-free bugs

    auto *new_page = reinterpret_cast<PhysicalMemList *>(virtual_page_address);
    new_page->next = free_pages_;
    free_pages_    = new_page;
}
