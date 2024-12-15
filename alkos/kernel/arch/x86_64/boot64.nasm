          ; Stack
          extern stack_bottom
          extern stack_top

          ; GDT64
          extern GDT64.Data

          ; Kernel Entry Point
          extern KernelMain

          ; Enabling extension
          extern enable_extensions

          global boot64
          section .text
          bits 64
boot64:
          mov ax, GDT64.Data
          mov ds, ax
          mov es, ax
          mov fs, ax
          mov gs, ax
          mov ss, ax

        ; TODO:
        ; 1. Enable interrupts
        ; 2. GDT, IDT, TSS, etc.
        ; 3. C++ runtime initialization

          call enable_extensions

          call KernelMain
          mov r10, rax

          ; clear screen
          mov edi, 0xb8000
          mov ecx, 80*25
          xor eax, eax
          rep stosd

          mov rdi, 0xb8000 ; VGA text buffer
          mov rcx, 80 * 25 ; 80 columns, 25 rows
          mov ah, 0x08 ;
          lea rsi, [r10]
          call write_string
          .hang:
          hlt
          jmp .hang

write_string:
          lodsb       ; load byte from rsi into al
          test al, al ; check if null terminator
          jz .done
          mov word [rdi], ax
          add rdi, 2
          loop write_string
.done:
          ret
