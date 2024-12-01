          bits 32
          %include "error_codes.nasm"

PAGING_BIT          equ 1 << 31
PAE_BIT             equ 1 << 5
LONG_MODE_BIT       equ 1 << 8

PROTECTED_MODE_BIT  equ 1 << 31
EFER_MSR            equ 0xC0000080

          extern p4_table
          section .text32
          global enable_paging_and_long_mode
enable_paging_and_long_mode:
          ; Load the PML4 table into CR3
          mov eax, p4_table
          mov cr3, eax

          ; TODO: THESE TWO LINES ARE THE PROBLEM
;          ; Enable PAE
          mov eax, cr4
          or eax, PAE_BIT
          mov cr4, eax

          ; Note: Processors starting from Ice Lake support 5-level paging

          ; To enable PAGING we need to set the LME bit in the EFER MSR first
          ; So there is no way to really enable paging without enabling long mode
          ; I mean we could just enable it and not enter protected mode
          ; But that could bring unexpected behavior - I think

          ; Set the LME (Long Mode Enable) bit in the
          ; EFER (Extended Feature Enable Register)
          ; MSR (Model Specific Register)
          mov ecx, EFER_MSR
          rdmsr
          or eax, LONG_MODE_BIT
          wrmsr

;          ; Enable paging
          mov eax, cr0
          or eax, PAGING_BIT | PROTECTED_MODE_BIT
          mov cr0, eax


          mov al, NO_ERROR
          ret

