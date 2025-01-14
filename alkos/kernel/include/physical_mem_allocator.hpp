#ifndef ALK_OS_KERNEL_INCLUDE_PHYSICAL_MEM_ALLOCATOR_HPP_
#define ALK_OS_KERNEL_INCLUDE_PHYSICAL_MEM_ALLOCATOR_HPP_

#include <todo.hpp>
#include <types.hpp>

TODO_WHEN_MULTITHREADING

class PhysicalMemAllocator
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

    PhysicalMemAllocator();
    ~PhysicalMemAllocator();

    public:
    // Delete copy constructor and copy assignment operator
    PhysicalMemAllocator(const PhysicalMemAllocator &other)            = delete;
    PhysicalMemAllocator &operator=(const PhysicalMemAllocator &other) = delete;

    // Delete move constructor and move assignment operator
    PhysicalMemAllocator(PhysicalMemAllocator &&other)            = delete;
    PhysicalMemAllocator &operator=(PhysicalMemAllocator &&other) = delete;

    //------------------------------------------------------------------------------//
    //                            Singleton Access Method                           //
    //------------------------------------------------------------------------------//

    /**
     * @brief GetInstance - Provides access to the single instance of PhysicalMemAllocator.
     *
     * @return A reference to the singleton instance.
     */
    static PhysicalMemAllocator &GetInstance()
    {
        static PhysicalMemAllocator instance;
        return instance;
    }

    //------------------------------------------------------------------------------//
    //                                Public Methods                                //
    //------------------------------------------------------------------------------//

    /**
     * @brief FreeRange - Marks a range of physical memory as free for the allocator to use.
     *
     * @param start - The start of the range.
     * @param end - The end of the range.
     */
    void FreeRange(byte *start, byte *end);

    /**
     * @brief AllocatePage - Allocates a single page of physical memory.
     */
    byte *AllocatePage();

    /**
     * @brief FreePage - Marks a single page of physical memory as free for the allocator to use.
     *
     * @param physical_page - The page to free.
     */
    void FreePage(byte *physical_page);

    //------------------------------------------------------------------------------//
    //                                Public Fields                                 //
    //------------------------------------------------------------------------------//

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
    static constexpr u64 kPhysicalPageSize = 4096;
    PhysicalMemList *free_pages_;
};

#endif  // ALK_OS_KERNEL_INCLUDE_PHYSICAL_MEM_ALLOCATOR_HPP_
