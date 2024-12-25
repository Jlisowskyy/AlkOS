#ifndef STDIO_H_
#define STDIO_H_

#include <stdarg.h>
#include <stddef.h>

int snprintf(char *str, size_t size, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list va);

#endif //STDIO_H_
