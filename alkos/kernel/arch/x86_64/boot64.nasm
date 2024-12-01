          bits 64

          ; Stack
          extern stack_bottom
          extern stack_top

          ; GDT64
          extern GDT64
          extern GDT64.Pointer
          extern GDT64.Code
          extern GDT64.Data

          global boot64
          section .text
boot64:
          cli
          mov esp, stack_top
          mov ebp, stack_top

          mov ax, GDT64.Data
          mov ds, ax
          mov es, ax
          mov fs, ax
          mov gs, ax
          mov ss, ax

.hang:
          hlt
          jmp .hang