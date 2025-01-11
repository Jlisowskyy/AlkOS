#ifndef ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_ELF_HPP_
#define ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_ELF_HPP_

#include <stdint.h>
#include <tags.hpp>

// ELF-64 Structures

static constexpr unsigned char ELF_MAGIC0 = 0x7F;
static constexpr unsigned char ELF_MAGIC1 = 'E';
static constexpr unsigned char ELF_MAGIC2 = 'L';
static constexpr unsigned char ELF_MAGIC3 = 'F';

// Taken from glibc https://github.com/lattera/glibc/blob/master/elf/elf.h#L60
// And https://krinkinmu.github.io/2020/11/15/loading-elf-image.html

/**
 * @brief ELF-64 header structure
 *
 * This structure represents the ELF-64 header.
 * It is used to identify the file as an ELF file and to locate the program header table.
 * The program header table contains information about the segments in the file.
 * The entry point of the program is also stored in this header. Among other useful information.
 */
struct Elf64_Ehdr {
    unsigned char e_ident[16]; /* Magic number and other info */
    uint16_t e_type;           /* Object file type */
    uint16_t e_machine;        /* Architecture */
    uint32_t e_version;        /* Object file version */
    uint64_t e_entry;          /* Entry point virtual address */
    uint64_t e_phoff;          /* Program header table file offset */
    uint64_t e_shoff;          /* Section header table file offset */
    uint32_t e_flags;          /* Processor-specific flags */
    uint16_t e_ehsize;         /* ELF header size in bytes */
    uint16_t e_phentsize;      /* Program header table entry size */
    uint16_t e_phnum;          /* Program header table entry count */
    uint16_t e_shentsize;      /* Section header table entry size */
    uint16_t e_shnum;          /* Section header table entry count */
    uint16_t e_shstrndx;       /* Section header string table index */
} __attribute__((packed));

/**
 * @brief ELF-64 program header structure
 *
 * This structure represents an entry in the program header table.
 * It contains information about a segment in the file.
 * Such as the offset in the file, the virtual address in memory, the physical address, the size in
 * the file, the size in memory, and the alignment etc.
 *
 */
struct Elf64_Phdr {
    uint32_t p_type;   /* Segment type */
    uint32_t p_flags;  /* Segment flags */
    uint64_t p_offset; /* Segment file offset */
    uint64_t p_vaddr;  /* Segment virtual address */
    uint64_t p_paddr;  /* Segment physical address */
    uint64_t p_filesz; /* Segment size in file */
    uint64_t p_memsz;  /* Segment size in memory */
    uint64_t p_align;  /* Segment alignment */
} __attribute__((packed));

#define PT_LOAD 1  // Loadable segment

// Function to load ELF-64 kernel
void* LoadElf64Module(uint8_t* elf_start, uint8_t* elf_end);

#endif  // ALK_OS_KERNEL_ARCH_X86_64_BOOT32_INCLUDE_ELF_HPP_
