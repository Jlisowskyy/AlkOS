          bits 32
; Multiboot2 must leave EAX with a magic value in it to indicate that the system was booted
; by a Multiboot2-compliant boot loader.
; Source:
; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#EFI-amd64-machine-state-with-boot-services-enabled
          %include "error_codes.nasm"

MULTIBOOT_HEADER_MAGIC equ 0x36D76289
          section .text32
          global check_multiboot
check_multiboot:
          cmp eax, MULTIBOOT_HEADER_MAGIC
          jne .no_multiboot
          mov al, NO_ERROR
          ret
.no_multiboot:
          mov al, ERROR_NO_MULTIBOOT
          ret