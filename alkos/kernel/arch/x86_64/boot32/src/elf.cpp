//
// Created by wookie on 1/4/25.
//
#include <arch_utils.hpp>
#include <terminal.hpp>
#include <tags.hpp>
#include <elf.hpp>
#include <libc.hpp>

#define MAYBE_UNUSED(x) (void)(x)

bool load_elf64_kernel(uint8_t* elf_data, size_t elf_size, uint64_t& entry_point) {
    MAYBE_UNUSED(elf_size);
    MAYBE_UNUSED(entry_point);
    MAYBE_UNUSED(elf_data);
    return true;
}
