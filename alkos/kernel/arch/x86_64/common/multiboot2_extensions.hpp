#ifndef ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_MULTIBOOT2_EXTENSIONS_HPP_
#define ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_MULTIBOOT2_EXTENSIONS_HPP_

#include <multiboot2.h>

/**
 * @brief Get the tag name from the tag type.
 *
 * @param type The tag type.
 * @return const char* The tag name.
 */
const char* GetTagName(unsigned int type);

/**
 * @brief Find the kernel module in the multiboot tags.
 *
 * @param multiboot_info_addr The address of the multiboot info structure.
 * @return multiboot_tag_module* The multiboot tag of the kernel module.
 */
multiboot_tag_module* FindKernelModule(void* multiboot_info_addr);

/**
 * @brief Find the memory map tag in the multiboot tags.
 *
 * @param multiboot_info_addr The address of the multiboot info structure.
 * @return multiboot_memory_map_t* The multiboot memory map tag.
 */
multiboot_tag_mmap* FindMemoryMap(void* multiboot_info_addr);

#endif  // ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_MULTIBOOT2_EXTENSIONS_HPP_
