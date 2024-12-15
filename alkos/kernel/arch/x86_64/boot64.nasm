          ; Stack
          extern stack_bottom
          extern stack_top

          ; GDT64
          extern GDT64.Data

          ; vga code
          extern terminal_initialize

          ; Kernel Entry Point
          extern KernelMain

          ; Enabling osxsave
          extern enable_osxsave

          ; Enabling extension
          extern enable_extensions

          global boot64
          global os_hang
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

          sub rsp, 32 ; shadow space

          ; Pre-kernel init code
          call terminal_initialize

          ; Setuping CPU features
          call enable_osxsave
          call enable_extensions

          ; Kernel Entry Point
          call KernelMain

          ; Infinite loop
os_hang:
          hlt
          jmp os_hang

