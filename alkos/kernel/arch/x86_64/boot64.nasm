
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
          bits 64
boot64:
          mov rax, 0x2f592f412f4b2f4f
          mov qword [0xb8000], rax
          hlt

.hang:
          hlt
          jmp .hang