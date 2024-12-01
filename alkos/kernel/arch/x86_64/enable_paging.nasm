          bits 32
          %include "error_codes.nasm"

PROTECTED_MODE_BIT  equ 1 << 0
PAGING_BIT          equ 1 << 31
PAE_BIT             equ 1 << 5

          extern p4_table
          section .text32
          global enable_paging
enable_paging:
          ; Enable PAE
          mov eax, cr4
          or eax, PAE_BIT
          mov cr4, eax

          ; Load the PML4 table into CR3
          mov eax, p4_table
          mov cr3, eax

          ; Enable paging
          mov eax, cr0
          or eax, PAGING_BIT
          mov cr0, eax

          ; Note: Processors starting from Ice Lake support 5-level paging

          mov al, NO_ERROR
          ret

