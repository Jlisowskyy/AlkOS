//
// Created by wookie on 1/4/25.
//
#include <stdint.h>
#include <libc.hpp>

void *memcpy(void *dest, const void *src, size_t n)
{
    if (!dest || !src)
        return nullptr;

    auto *d       = static_cast<unsigned char *>(dest);
    const auto *s = static_cast<const unsigned char *>(src);

    for (size_t i = 0; i < n; ++i)
    {
        d[i] = s[i];
    }
    return dest;
}


void *memset(void *dest, int c, size_t n)
{
    if (!dest)
        return nullptr;

    auto *d = static_cast<unsigned char *>(dest);
    auto uc = static_cast<unsigned char>(c);

    for (size_t i = 0; i < n; ++i)
    {
        d[i] = uc;
    }
    return dest;
}

void uint32_to_string(uint32_t value, char* buffer)
{
    uint32_t i = 0;
    do {
        buffer[i++] = '0' + value % 10;
        value /= 10;
    } while (value > 0);
    buffer[i] = '\0';

    for (uint32_t j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}
