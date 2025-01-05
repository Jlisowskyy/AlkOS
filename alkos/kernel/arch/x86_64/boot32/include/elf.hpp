//
// Created by wookie on 1/4/25.
//

#ifndef ALKOS_ELF_LOADER_H
#define ALKOS_ELF_LOADER_H
#include <stdint.h>
#include <tags.hpp>

// ELF-64 Structures
struct Elf64_Ehdr {
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;      // Entry point
    uint64_t e_phoff;      // Program header table offset
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed));

struct Elf64_Phdr {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;     // Offset in file
    uint64_t p_vaddr;      // Virtual address in memory
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} __attribute__((packed));

#define PT_LOAD 1

// Function to load ELF-64 kernel
bool load_elf64_kernel(uint8_t* elf_data, size_t elf_size, uint64_t& entry_point);

#endif //ALKOS_ELF_LOADER_H
