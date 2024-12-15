          ; Stack
          extern stack_bottom
          extern stack_top

          ; GDT64
          extern GDT64.Data

          ; Serial
          extern serial_init
          extern serial_puts

          ; Message
          extern MESSAGE_INFO_IN_64

          ; Kernel Entry Point
          extern KernelMain

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

          mov rdi, MESSAGE_INFO_IN_64
          call serial_puts

          call KernelMain

          cli
.hang:
          hlt
          jmp .hang
