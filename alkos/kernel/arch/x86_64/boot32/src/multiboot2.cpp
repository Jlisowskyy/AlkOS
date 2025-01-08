#include <multiboot2.h>
#include <terminal.hpp>
#include <tags.hpp>
#include <types.hpp>
#include <libc.hpp>

extern char text_buffer[1024];

multiboot_tag_module* FindKernelModule(void* multiboot_info_addr) {
    multiboot_tag_module* kernel_module = nullptr;
    multiboot_tag* tag;
    for (tag = (multiboot_tag*)(multiboot_info_addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7))) {
        TerminalWriteString(INFO_TAG "Found tag: ");
        TerminalWriteString(GetTagName(tag->type));
        TerminalWriteString("\n");

        if(tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
            multiboot_tag_module* module = (multiboot_tag_module*)tag;
            kernel_module = module;

            // TODO: Modules should be loaded with a specific command line
            // so that the loader knows which module is the kernel
            // but for now, we will just take the last module as the kernel

            TerminalWriteString(INFO_TAG "Module loaded at: ");
            Uint32ToString(module->mod_start, text_buffer);
            TerminalWriteString(text_buffer);
            TerminalWriteString(" - ");
            Uint32ToString(module->mod_end, text_buffer);
            TerminalWriteString(text_buffer);
            TerminalWriteString("\n");
            TerminalWriteString(INFO_TAG "With command line: ");
            if ( (char*)module->cmdline == "\0") {
                TerminalWriteString((char*)module->cmdline);
            } else {
                TerminalWriteString("no command line provided");
            }
            TerminalWriteString("\n");
        }
    }
    return kernel_module;
}
const char* GetTagName(unsigned int type) {
    switch (type) {
        case MULTIBOOT_TAG_TYPE_END:
            return "MULTIBOOT_TAG_TYPE_END";
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            return "MULTIBOOT_TAG_TYPE_CMDLINE";
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            return "MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME";
        case MULTIBOOT_TAG_TYPE_MODULE:
            return "MULTIBOOT_TAG_TYPE_MODULE";
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            return "MULTIBOOT_TAG_TYPE_BASIC_MEMINFO";
        case MULTIBOOT_TAG_TYPE_BOOTDEV:
            return "MULTIBOOT_TAG_TYPE_BOOTDEV";
        case MULTIBOOT_TAG_TYPE_MMAP:
            return "MULTIBOOT_TAG_TYPE_MMAP";
        case MULTIBOOT_TAG_TYPE_VBE:
            return "MULTIBOOT_TAG_TYPE_VBE";
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            return "MULTIBOOT_TAG_TYPE_FRAMEBUFFER";
        case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
            return "MULTIBOOT_TAG_TYPE_ELF_SECTIONS";
        case MULTIBOOT_TAG_TYPE_APM:
            return "MULTIBOOT_TAG_TYPE_APM";
        case MULTIBOOT_TAG_TYPE_EFI32:
            return "MULTIBOOT_TAG_TYPE_EFI32";
        case MULTIBOOT_TAG_TYPE_EFI64:
            return "MULTIBOOT_TAG_TYPE_EFI64";
        case MULTIBOOT_TAG_TYPE_SMBIOS:
            return "MULTIBOOT_TAG_TYPE_SMBIOS";
        case MULTIBOOT_TAG_TYPE_ACPI_OLD:
            return "MULTIBOOT_TAG_TYPE_ACPI_OLD";
        case MULTIBOOT_TAG_TYPE_ACPI_NEW:
            return "MULTIBOOT_TAG_TYPE_ACPI_NEW";
        case MULTIBOOT_TAG_TYPE_NETWORK:
            return "MULTIBOOT_TAG_TYPE_NETWORK";
        case MULTIBOOT_TAG_TYPE_EFI_MMAP:
            return "MULTIBOOT_TAG_TYPE_EFI_MMAP";
        case MULTIBOOT_TAG_TYPE_EFI_BS:
            return "MULTIBOOT_TAG_TYPE_EFI_BS";
        case MULTIBOOT_TAG_TYPE_EFI32_IH:
            return "MULTIBOOT_TAG_TYPE_EFI32_IH";
        case MULTIBOOT_TAG_TYPE_EFI64_IH:
            return "MULTIBOOT_TAG_TYPE_EFI64_IH";
        case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
            return "MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR";
        default:
            return "UNKNOWN";
    }
}