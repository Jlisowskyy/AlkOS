#ifndef ARCH_X86_64_INCLUDE_ARCH_UTILS_HPP_
#define ARCH_X86_64_INCLUDE_ARCH_UTILS_HPP_

inline void OsHang() {
    asm volatile("cli");
    while (1) {
        asm volatile("hlt");
    }
}

#endif // ARCH_X86_64_INCLUDE_ARCH_UTILS_HPP_
