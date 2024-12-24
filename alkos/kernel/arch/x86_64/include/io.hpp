#ifndef ARCH_X86_64_IO_HPP_
#define ARCH_X86_64_IO_HPP_

#include <defines.hpp>
#include <types.hpp>

WRAP_CALL byte inb(const u16 port)
{
    byte v;
    __asm__ volatile("inb %w1, %0" : "=a"(v) : "Nd"(port));
    return v;
}

WRAP_CALL u16 inw(const u16 port)
{
    u16 v;
    __asm__ volatile("inw %w1, %0" : "=a"(v) : "Nd"(port));
    return v;
}

WRAP_CALL u32 inl(const u16 port)
{
    u32 v;
    __asm__ volatile("inl %w1, %0" : "=a"(v) : "Nd"(port));
    return v;
}

WRAP_CALL void outb(const u16 port, const u8 v) { __asm__ volatile("outb %b0, %w1" : : "a"(v), "Nd"(port)); }

WRAP_CALL void outw(const u16 port, const u16 v) { __asm__ volatile("outw %w0, %w1" : : "a"(v), "Nd"(port)); }

WRAP_CALL void outl(const u16 port, const u32 v) { __asm__ volatile("outl %0, %w1" : : "a"(v), "Nd"(port)); }

#endif // ARCH_X86_64_IO_HPP_
