//
// Created by wookie on 1/4/25.
//

#ifndef ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_ELF_HPP
#define ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_ELF_HPP

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
 * @brief Copy memory area as if it was first copied to a temporary area and then copied back to dest
 *
 * @param dest Destination memory area
 * @param src Source memory area
 * @param n Number of bytes to copy
 * @return void* Pointer to the destination memory area
 */

void *memmove(void *dest, const void *src, size_t n);
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
 * @brief Compare two memory areas
 *
 * @param s1 First memory area
 * @param s2 Second memory area
 * @param n Number of bytes to compare
 * @return int 0 if the memory areas are equal\n
 * \<0 if the first differing byte of s1 is less than the respective one of s2\n
 * \>0 if the first differing byte of s1 is greater than the respective one of s2
 *
 * @note If s1 or s2 is a null pointer, the function returns 0
 * @note if n is 0, the function returns 0
 */
int memcmp(const void *s1, const void *s2, size_t n);

/**
 * @brief Compare two strings ignoring case
 *
 * @param s1 First string
 * @param s2 Second string
 * @return int 0 if the strings are equal\n
 * \<0 if the first differing character of s1 is less than the respective one of s2\n
 * \>0 if the first differing character of s1 is greater than the respective one of s2
 */
int strcasecmp(const char *s1, const char *s2);

/**
 * @brief extract tokens from strings
 *
 * @param str The string to extract tokens from
 * @param delim The delimiter characters
 * @return char* The next token in the string
 */
char* strtok(char *str, const char *delim);

/**
 * @brief Find the length of the initial segment of str1 which consists only of characters not in str2
 *
 * @param str The string to search
 * @param delim The delimiter characters
 * @return size_t The length of the initial segment of str1 which consists only of characters not in str2
 */

char *strcpy(char *dest, const char *src);
/**
 * @brief Copy a string
 *
 * @param dest Destination string
 * @param src Source string
 * @param n Number of characters to copy
 * @return char* Pointer to the destination string
 */
char* strncpy(char *dest, const char *src, size_t n);

#endif // ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_ELF_HPP
