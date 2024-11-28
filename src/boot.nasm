; Entry point for the kernel
extern kernel_main

; Constants for Multiboot header
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; Multiboot header
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

; The multiboot standard does not define the value of the stack pointer register.
; The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

; The bootloader has loaded us into 32-bit protected mode on a x86
; machine. Interrupts are disabled. Paging is disabled. The processor
; state is as defined in the multiboot standard. The kernel has full
; control of the CPU.
section .text
global _start:function (_start.end - _start)
_start:
    ; Set up the stack pointer
    mov esp, stack_top

    ; Enable various CPU features below:

    ; TODO:
    ; 1. Enable paging
    ; 2. Enable interrupts
    ; 3. Enable long mode
    ; 4. Enable Floating Point Unit (FPU)
    ; 5. Enable Instruction Set Extensions (SSE, AVX, etc.)
    ; 6. C++ runtime initialization

    ; call kernel main function
    ; Note: Stack already aligned to 16 bytes

    ; Call kernel entry point
    call kernel_main

    ; Disable interrupts. For future use.
    cli

    ; Halt the CPU in an infinite loop.
.hang:	hlt
    jmp .hang
.end: