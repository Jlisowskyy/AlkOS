#ifndef ALK_OS_KERNEL_INCLUDE_PHYSICAL_MEM_ALLOCATOR_HPP_
#define ALK_OS_KERNEL_INCLUDE_PHYSICAL_MEM_ALLOCATOR_HPP_

#include <todo.hpp>
#include <types.hpp>

TODO_WHEN_MULTITHREADING

class PhysicalMemoryManager
{
    private:
    //------------------------------------------------------------------------------//
    //                                Internal Types                                //
    //------------------------------------------------------------------------------//

    struct PhysicalMemList {
        PhysicalMemList *next;
    };

    //------------------------------------------------------------------------------//
    //                            Creation & Destruction                            //
    //------------------------------------------------------------------------------//

    public:
    /**
     * @brief PhysicalMemoryManager - The default constructor should only ever be used in tests!!!
     * Use GetInstance() to get the singleton instance.
     */
    PhysicalMemoryManager();
    ~PhysicalMemoryManager();

    // Delete copy constructor and copy assignment operator
    PhysicalMemoryManager(const PhysicalMemoryManager &other)            = delete;
    PhysicalMemoryManager &operator=(const PhysicalMemoryManager &other) = delete;

    // Delete move constructor and move assignment operator
    PhysicalMemoryManager(PhysicalMemoryManager &&other)            = delete;
    PhysicalMemoryManager &operator=(PhysicalMemoryManager &&other) = delete;

    //------------------------------------------------------------------------------//
    //                            Singleton Access Method                           //
    //------------------------------------------------------------------------------//

    /**
     * @brief GetInstance - Provides access to the single instance of PhysicalMemAllocator.
     *
     * @return A reference to the singleton instance.
     */
    static PhysicalMemoryManager &GetInstance();

    //------------------------------------------------------------------------------//
    //                                Public Methods                                //
    //------------------------------------------------------------------------------//

    /**
     * @brief FreeRange - Marks a range of physical memory bounded by two virtual addresses as
     * free. There is no way to operate on physical addresses directly, so it is upon the user
     * to ensure that the range is valid.
     *
     * @param virtual_start_address - The start of the range.
     * @param virtual_end_address - The end of the range.
     */
    void FreeRange(byte *virtual_start_address, const byte *virtual_end_address);

    /**
     * @brief AllocatePage - Allocates a single page of physical memory.
     */
    byte *AllocatePage();

    /**
     * @brief FreePage - Marks a single page of physical memory as free for the allocator to use.
     *
     * @param virtual_page_address - The page to free.
     */
    void FreePage(byte *virtual_page_address);

    //------------------------------------------------------------------------------//
    //                                Public Fields                                 //
    //------------------------------------------------------------------------------//

    static constexpr u64 kPhysicalPageSize = 4096;

    private:
    //------------------------------------------------------------------------------//
    //                               Private Methods                                //
    //------------------------------------------------------------------------------//

    static constexpr u64 AlignAddressToPhysicalPageSize(u64 address)
    {
        return (address + kPhysicalPageSize - 1) & ~(kPhysicalPageSize - 1);
    }

    //------------------------------------------------------------------------------//
    //                               Private Fields                                 //
    //------------------------------------------------------------------------------//

    PhysicalMemList *free_pages_;
};

#endif  // ALK_OS_KERNEL_INCLUDE_PHYSICAL_MEM_ALLOCATOR_HPP_
