          bits 32
          %include "error_codes.nasm"

LONG_MODE_BIT       equ 1 << 8
PROTECTED_MODE_BIT  equ 1 << 31
EFER_MSR            equ 0xC0000080

          section .text32
          global enable_long_mode
enable_long_mode:
          ; Set the LME (Long Mode Enable) bit in the
          ; EFER (Extended Feature Enable Register)
          ; MSR (Model Specific Register)
          mov ecx, EFER_MSR
          rdmsr
          or eax, LONG_MODE_BIT
          wrmsr

          ; Enable protected mode
          mov eax, cr0
          or eax, PROTECTED_MODE_BIT
          mov cr0, eax

          mov al, NO_ERROR
          ret
