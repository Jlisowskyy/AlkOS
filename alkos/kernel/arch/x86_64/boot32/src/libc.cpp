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

void *memmove(void *dest, const void *src, size_t n)
{
    if (!dest || !src)
        return nullptr;

    auto *d       = static_cast<unsigned char *>(dest);
    const auto *s = static_cast<const unsigned char *>(src);

    if (d > s)
    {
        for (size_t i = n; i > 0; --i)
        {
            d[i - 1] = s[i - 1];
        }
    }
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            d[i] = s[i];
        }
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

int memcmp(const void *s1, const void *s2, size_t n)
{
    if (!s1 || !s2)
        return 0;
    if (n == 0)
        return 0;

    const auto *byte1 = static_cast<const unsigned char *>(s1);
    const auto *byte2 = static_cast<const unsigned char *>(s2);

    for (size_t i = 0; i < n; ++i)
    {
        if (byte1[i] < byte2[i])
            return -1;
        if (byte1[i] > byte2[i])
            return 1;
    }
    return 0;
}

int strcasecmp(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return 0;

    while (*s1 && *s2)
    {
        if (*s1 != *s2)
        {
            if (*s1 >= 'A' && *s1 <= 'Z')
            {
                if (*s1 + 32 != *s2)
                    return *s1 - *s2;
            }
            else if (*s1 >= 'a' && *s1 <= 'z')
            {
                if (*s1 - 32 != *s2)
                    return *s1 - *s2;
            }
            else
            {
                return *s1 - *s2;
            }
        }
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}

int strcmp(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return 0;

    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            return *s1 - *s2;
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}

size_t strspn(const char *str, const char *delim)
{
    if (!str || !delim)
        return 0;

    size_t count = 0;
    while (*str)
    {
        bool found = false;
        for (size_t i = 0; delim[i]; ++i)
        {
            if (*str == delim[i])
            {
                found = true;
                break;
            }
        }
        if (!found)
            return count;
        ++count;
        ++str;
    }
    return count;
}

size_t strcspn(const char *str, const char *delim)
{
    if (!str || !delim)
        return 0;

    size_t count = 0;
    while (*str)
    {
        for (size_t i = 0; delim[i]; ++i)
        {
            if (*str == delim[i])
                return count;
        }
        ++count;
        ++str;
    }
    return count;
}

char *strtok(char *str, const char *delim)
{
    static char *last = nullptr;

    if (!str && !last)
        return nullptr;

    if (!str)
        str = last;

    str += strspn(str, delim);

    if (*str == '\0')
        return nullptr;

    last = str + strcspn(str, delim);

    if (*last != '\0')
        *last++ = '\0';

    return str;
}

char *strcpy(char *dest, const char *src)
{
    if (!dest || !src)
        return nullptr;

    char *d = dest;
    while (*src)
    {
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    if (!dest || !src)
        return nullptr;

    char *d = dest;
    while (*src && n--)
    {
        *d++ = *src++;
    }
    *d = '\0';
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
