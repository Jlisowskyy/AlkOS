#include <physical_memory_manager.hpp>
#include <test_module/test.hpp>

class PhysicalMemoryManagerTest : public TestGroupBase
{
    protected:
    static constexpr size_t kNumPages = 10;
    byte physical_memory_mock[kNumPages * PhysicalMemoryManager::kPhysicalPageSize]{};
};

// ------------------------------
// Free Tests
// ------------------------------

TEST_F(PhysicalMemoryManagerTest, FreePage)
{
    PhysicalMemoryManager allocator{};
    allocator.FreePage(physical_memory_mock);
    EXPECT_EQ(allocator.AllocatePage(), physical_memory_mock);
}

TEST_F(PhysicalMemoryManagerTest, FreeRange)
{
    PhysicalMemoryManager allocator{};
    byte *end = physical_memory_mock + kNumPages * PhysicalMemoryManager::kPhysicalPageSize;
    allocator.FreeRange(physical_memory_mock, end);
    for (size_t i = 0; i < kNumPages; i++) {
        EXPECT_EQ(
            allocator.AllocatePage(),
            physical_memory_mock + i * PhysicalMemoryManager::kPhysicalPageSize
        );
    }
}

TEST_F(PhysicalMemoryManagerTest, FreeRangeTwice)
{
    PhysicalMemoryManager allocator{};
    byte *end = physical_memory_mock + kNumPages * PhysicalMemoryManager::kPhysicalPageSize;
    allocator.FreeRange(physical_memory_mock, end);
    for (size_t i = 0; i < kNumPages; i++) {
        EXPECT_EQ(
            allocator.AllocatePage(),
            physical_memory_mock + i * PhysicalMemoryManager::kPhysicalPageSize
        );
    }

    allocator.FreeRange(physical_memory_mock, end);
    for (size_t i = 0; i < kNumPages; i++) {
        EXPECT_EQ(
            allocator.AllocatePage(),
            physical_memory_mock + i * PhysicalMemoryManager::kPhysicalPageSize
        );
    }
}

FAIL_TEST_F(PhysicalMemoryManagerTest, NoFreePages)
{
    PhysicalMemoryManager allocator{};
    allocator.AllocatePage();  // No free pages so expect KernelPanic
}

FAIL_TEST_F(PhysicalMemoryManagerTest, FreeNullPage)
{
    PhysicalMemoryManager allocator{};
    allocator.FreePage(nullptr);  // Freeing a null page should panic
}

FAIL_TEST_F(PhysicalMemoryManagerTest, FreeUnalignedPage)
{
    PhysicalMemoryManager allocator{};
    allocator.FreePage(physical_memory_mock + 1);  // Freeing an unaligned page should panic
}

// ------------------------------
// Allocate Tests
// ------------------------------

TEST_F(PhysicalMemoryManagerTest, AllocateWhenNoFreePages)
{
    PhysicalMemoryManager allocator{};
    EXPECT_EQ(allocator.AllocatePage(), nullptr);
}

// ------------------------------
// Mixed Tests
// ------------------------------

TEST_F(PhysicalMemoryManagerTest, SeriesOfRandomFreeAndAllocations)
{
    PhysicalMemoryManager allocator{};
    byte *end = physical_memory_mock + kNumPages * PhysicalMemoryManager::kPhysicalPageSize;
    allocator.FreeRange(physical_memory_mock, end);

    allocator.AllocatePage();                                   // Page 0
    allocator.AllocatePage();                                   // Page 1
    allocator.AllocatePage();                                   // Page 2
    allocator.FreePage(physical_memory_mock);                   // Free page 0
    EXPECT_EQ(allocator.AllocatePage(), physical_memory_mock);  // Page 0
    allocator.FreePage(
        physical_memory_mock + PhysicalMemoryManager::kPhysicalPageSize
    );  // Free page 1
    EXPECT_EQ(
        allocator.AllocatePage(), physical_memory_mock + PhysicalMemoryManager::kPhysicalPageSize
    );  // Page 1
}
