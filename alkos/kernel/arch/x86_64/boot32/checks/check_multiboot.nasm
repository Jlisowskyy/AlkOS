; Multiboot2 must leave EAX with a magic value in it to indicate that the system was booted
; by a Multiboot2-compliant boot loader.
; Source:
; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#EFI-amd64-machine-state-with-boot-services-enabled

          bits 32

          %include "return_codes.nasm"

          MULTIBOOT_HEADER_MAGIC equ 0x36d76289

          section .text
          global check_multiboot
check_multiboot:
          cmp eax, MULTIBOOT_HEADER_MAGIC
          jne .no_multiboot
          mov eax, SUCCESS_CHECK_MULTIBOOT
          ret
.no_multiboot:
          mov eax, ERROR_NO_MULTIBOOT
          ret
