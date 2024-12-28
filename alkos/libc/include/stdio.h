#ifndef STDIO_H_
#define STDIO_H_

#include <stdarg.h>
#include <stddef.h>

/**
 *  Writes formatted output to a character array (*str) up to a maximum amount of characters (size)
 */
int snprintf(char *str, size_t size, const char *format, ...);

/**
 *  Writes formatted data from a variable argument list (va) to a sized buffer (str, size)
 */
int vsnprintf(char *str, size_t size, const char *format, va_list va);

#endif // STDIO_H_