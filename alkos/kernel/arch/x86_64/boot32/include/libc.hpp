//
// Created by wookie on 1/4/25.
//

#ifndef ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_LIBC_HPP
#define ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_LIBC_HPP

#include <stddef.h>

/**
 * @brief Copy memory area
 *
 * @param dest Destination memory area
 * @param src Source memory area
 * @param n Number of bytes to copy
 * @return void* Pointer to the destination memory area
 *
 * @note The memory areas must not overlap use memmove() instead
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * @brief Fill memory area with a constant byte
 *
 * @param dest Destination memory area
 * @param c Constant byte
 * @param n Number of bytes to fill
 * @return void* Pointer to the destination memory area
 */
void *memset(void *dest, int c, size_t n);

/**
 * @brief Converts a uint32_t to a string
 *
 * @param value The value to convert
 * @param buffer The buffer to store the string
 */
void uint32_to_string(uint32_t value, char* buffer);

#endif // ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_LIBC_HPP
