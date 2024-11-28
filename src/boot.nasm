          ; Entry point for the kernel
          extern kernel_main

          ; Constants for Multiboot header
MBALIGN   equ  1 << 0              ; align loaded modules on page boundaries
MEMINFO   equ  1 << 1              ; provide memory map
MBFLAGS   equ  MBALIGN | MEMINFO   ; this is the Multiboot 'flag' field
MAGIC     equ  0x1BADB002          ; 'magic number' lets bootloader find the header
CHECKSUM  equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; Multiboot header
section   .multiboot
          align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

; The multiboot standard does not define the value of the stack pointer register.
; The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section   .bss
          align 16
stack_bottom:
          resb 16384 ; 16 KiB
stack_top:

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
section   .text
global    _start:function (_start.end - _start)
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

          ; Enable various CPU features below:

          ; TODO:
          ; 1. Enable paging
          ; 2. Enable interrupts
          ; 3. Enable long mode
          ; 4. Enable Floating Point Unit (FPU)
          ; 5. Enable Instruction Set Extensions (SSE, AVX, etc.)
          ; 7. GDT, IDT, TSS, etc.
          ; 6. C++ runtime initialization


          ; Call kernel entry point
          ; Note: Stack already aligned to 16 bytes
          ; Note: that if you are building on Windows, C functions may have "_" prefix in assembly: _kernel_main
          call kernel_main


          ; Disable interrupts. For future use.
          cli

          ; Halt the CPU in an infinite loop.
.hang:	hlt
          jmp .hang
.end: