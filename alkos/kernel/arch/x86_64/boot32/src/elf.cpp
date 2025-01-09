#include <arch_utils.hpp>
#include <terminal.hpp>
#include <tags.hpp>
#include <elf.hpp>
#include <memory.h>

#define MAYBE_UNUSED(x) (void)(x)

extern char text_buffer[1024];

void* LoadElf64Module(uint8_t* elf_start, uint8_t* elf_end){
    TerminalWriteString(INFO_TAG "Loading ELF-64 module...\n");
    if (elf_start == nullptr || elf_end == nullptr) {
        TerminalWriteString(ERROR_TAG "Null ELF boundaries.\n");
        return nullptr;
    }
    if (elf_start >= elf_end) {
        TerminalWriteString(ERROR_TAG "Invalid ELF boundaries: start >= end.\n");
        return nullptr;
    }

    Elf64_Ehdr* ehdr = reinterpret_cast<Elf64_Ehdr*>(elf_start);


    // Verify ELF magic
    if (!(ehdr->e_ident[0] == ELF_MAGIC0 &&
          ehdr->e_ident[1] == ELF_MAGIC1 &&
          ehdr->e_ident[2] == ELF_MAGIC2 &&
          ehdr->e_ident[3] == ELF_MAGIC3))
    {
        TerminalWriteString(ERROR_TAG "File is not valid ELF.\n");
        return nullptr;
    }

    if (ehdr->e_machine != 0x3E /* EM_X86_64 */) {
        TerminalWriteString(ERROR_TAG "Unsupported machine type.\n");
        return nullptr;
    }

    size_t elf_size = static_cast<size_t>(elf_end - elf_start);
    if (ehdr->e_phoff + (ehdr->e_phnum * ehdr->e_phentsize) > elf_size) {
        TerminalWriteString(ERROR_TAG "Program header table out of file bounds.\n");
        return nullptr;
    }

    Elf64_Phdr* phdrs = reinterpret_cast<Elf64_Phdr*>(elf_start + ehdr->e_phoff);

    for (uint16_t i = 0; i < ehdr->e_phnum; i++) {
        const Elf64_Phdr* phdr = &phdrs[i];

        if (phdr->p_offset + phdr->p_filesz > elf_size) {
            TerminalWriteString(ERROR_TAG "Declared elf segment size in file exceeds the size of the ELF itself.\n");
            return nullptr;
        }

        // Only care about segments that should be loaded into memory
        if (phdr->p_type == PT_LOAD) {
            TerminalWriteString(INFO_TAG "Loading segment...\n");
            uint64_t segment_dist = phdr->p_vaddr;
            uint64_t segment_dist_size = phdr->p_memsz;
            uint64_t segment_source = (uint32_t)elf_start + phdr->p_offset;
            uint64_t segment_source_size = phdr->p_filesz;

            memcpy(reinterpret_cast<void*>(segment_dist),
                   reinterpret_cast<void*>(segment_source),
                   static_cast<size_t>(segment_source_size));

            // Zero out the rest of the segment
            if (segment_dist_size > segment_source_size) {
                memset(reinterpret_cast<void*>(segment_dist + segment_source_size),
                       0,
                       static_cast<size_t>(segment_dist_size - segment_source_size));
            }
            TerminalWriteString(SUCCESS_TAG "Loaded a LOAD segment.\n");
        }
    }

    // If all program headers were loaded, we have a valid entry point
    TerminalWriteString(SUCCESS_TAG "ELF-64 module loaded successfully.\n");
    return reinterpret_cast<void*>(ehdr->e_entry);
    return nullptr;
}
