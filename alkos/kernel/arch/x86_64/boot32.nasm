          ; TODO:
          ; 2. Enable interrupts
          ; 4. Enable Floating Point Unit (FPU)
          ; 5. Enable Instruction Set Extensions (SSE, AVX, etc.)
          ; 7. GDT, IDT, TSS, etc.
          ; 6. C++ runtime initialization

          bits 32

          ; Helper functions
          extern vga_print

          ; Error checking and handling
          extern check_multiboot
          extern check_cpuid
          extern check_long_mode
          extern check_and_handle_errors

          extern MESSAGE_ERROR_NO_LONG_MODE
          extern MESSAGE_ERROR_NO_CPUID
          extern MESSAGE_ERROR_UNKNOWN

          ; GDT64
          extern GDT64.Pointer
          extern GDT64.Code

          ; Stack
          extern stack_bottom
          extern stack_top

          ; Paging
          extern p1_table
          extern p2_table
          extern p3_table
          extern p4_table
          extern setup_page_tables
          extern enable_paging

          ; Long mode
          extern enable_long_mode

          ; Kernel Entry Point
          extern kernel_main

          ; Boot64 - entry point to 64-bit boot code - continuation of what is here
          extern boot64

; The multiboot standard does not define the value of the stack pointer register.
; The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
section   .text32
global    _start
_start:
          ; The bootloader has loaded us into 32-bit protected mode on a x86
          ; machine. Interrupts are disabled. Paging is disabled. The processor
          ; state is as defined in the multiboot standard. The kernel has full
          ; control of the CPU. The kernel can only make use of hardware features
          ; and any code it provides as part of itself. There's no printf
          ; function, unless the kernel provides its own <stdio.h> header and a
          ; printf implementation. There are no security restrictions, no
          ; safeguards, no debugging mechanisms, only what the kernel provides
          ; itself. It has absolute and complete power over the
          ; machine.

          ; To set up a stack, we set the esp register to point to the top of the
          ; stack (as it grows downwards on x86 systems). This is necessarily done
          ; in assembly as languages such as C cannot function without a stack.
          mov esp, stack_top

;          call check_multiboot
;          call check_and_handle_errors
          call check_cpuid
          call check_and_handle_errors
          call check_long_mode
          call check_and_handle_errors
          call setup_page_tables
          call check_and_handle_errors
          call enable_long_mode
          call check_and_handle_errors
          call enable_paging
          call check_and_handle_errors

          ; Jump to long mode
          lgdt [GDT64.Pointer]
          jmp GDT64.Code:boot64
