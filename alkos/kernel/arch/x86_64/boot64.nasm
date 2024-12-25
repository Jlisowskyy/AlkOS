          ; Stack
          extern stack_bottom
          extern stack_top

          ; Includes
          %include "puts.nasm"

          ; GDT64
          extern GDT64.Data

          ; Message
          extern MESSAGE_INFO_IN_64

          ; Totally basic initialization that must be done before calling _init
          extern PreKernelInit

          ; GCC compiler global constructors initialization
          extern _init
          extern _fini

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

          puts MESSAGE_INFO_IN_64

          sub rsp, 32 ; shadow space

          ; There all basic initialization should be done
          call PreKernelInit

          ; Invoke global constructors
          call _init

          ; Call actual kernel entry point
          call KernelMain

          ; Not actually needed (as we expect to never return from Kernel), but exists for completeness
          call _fini

          ; Infinite loop
os_hang:
          jmp os_hang
