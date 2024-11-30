          ; TODO:
          ; 2. Enable interrupts
          ; 4. Enable Floating Point Unit (FPU)
          ; 5. Enable Instruction Set Extensions (SSE, AVX, etc.)
          ; 7. GDT, IDT, TSS, etc.
          ; 6. C++ runtime initialization

          BITS 32
          ; Entry point for the kernel
          extern vga_print
          extern kernel_main
          extern check_cpuid
          extern check_long_mode
          extern check_and_handle_errors

          ; Constants for Multiboot header
MBALIGN   equ  1 << 0              ; align loaded modules on page boundaries
MEMINFO   equ  1 << 1              ; provide memory map
MBFLAGS   equ  MBALIGN | MEMINFO   ; this is the Multiboot 'flag' field
MAGIC     equ  0x1BADB002          ; 'magic number' lets bootloader find the header
CHECKSUM  equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; GDT constants
; Access bits
PRESENT    equ 1 << 7 ; Present
NOT_SYS    equ 0 << 4 ; Not a system segment
EXEC       equ 1 << 3 ; Executable
DC         equ 1 << 2 ; Direction/Conforming
RW         equ 1 << 1 ; Readable/Writable
AC         equ 1 << 0 ; Accessed
; Flags bits
GRAN_4K    equ 1 << 7 ; 4 KiB granularity
SZ_32      equ 1 << 6 ; 32-bit segment
LONG_MODE  equ 1 << 5 ; Long mode

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
          align 4096
p4_table:
          resb 4096 ; Page Map Level 4
p3_table:
          resb 4096 ; Page Directory Pointer Table
p2_table:
          resb 4096 ; Page Directory
p1_table:
          resb 4096 ; Page Table
stack_bottom:
          resb 16384 ; 16 KiB
stack_top:


section   .data
          align 8
GDT64:
          .Null:
          dq 0
          .Code: equ $ - GDT64
          dd 0xFFFF ; Limit
          db 0      ; Base
          db PRESENT | NOT_SYS | EXEC | RW ; Access
          db GRAN_4K | LONG_MODE | 0xF     ; Flags & Limit
          db 0      ; Base
          .Data: equ $ - GDT64
          dd 0xFFFF ; Limit
          db 0      ; Base
          db PRESENT | NOT_SYS | RW ; Access
          db GRAN_4K | SZ_32 | 0xF     ; Flags & Limit
          db 0      ; Base
          .Pointer:
          dw $ - GDT64 - 1 ; $ - Special symbol that evaluates to the current address
          dd GDT64
.End:

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

          call check_cpuid
          call check_and_handle_errors
          call check_long_mode
          call check_and_handle_errors

          ; PML4[0] = PDPT
          ; PDPT[0] = PD
          ; PD[0] = PT

          mov eax, p3_table
          or eax, 0x3 ; Present, Read/Write
          mov [p4_table], eax

          mov eax, p2_table
          or eax, 0x3 ; Present, Read/Write
          mov [p3_table], eax

          mov eax, p1_table
          or eax, 0x3 ; Present, Read/Write
          mov [p2_table], eax

          ; Identity map the first 2 MiB of memory
          mov ebx, 0x00000003
          mov ecx, 512

.map_p2_table:
          ; Map ecx-th page to a page that starts at 2 MiB * ecx
          mov eax, 0x200000 ; 2 MiB
          mul ecx           ; start address of ecx-th page
          or eax, 0b10000011 ; Present, Read/Write, and something (User/Supervisor?)
          mov [p2_table + ecx * 8], eax

          inc ecx
          loop .map_p2_table

.enable_paging:
          ; Setup Paging for Long Mode
          mov edi, p4_table
          mov cr3, edi

          xor eax, eax
          mov ecx, 4096
          rep stosd ; Zero out the PML4 table

          ; Enable PAE-paging
          mov eax, cr4
          or eax, 1 << 5 ; Set PAE bit
          mov cr4, eax

          ; Procesors starting from Ice Lake support 5-level paging

          ; Enter compatibility mode
          ; Set the LME (Long Mode Enable) bit in the
          ; EFER (Extended Feature Enable Register)
          ; MSR (Model Specific Register)
          mov ecx, 0xC0000080 ; EFER MSR
          rdmsr ;
          or eax, 1 << 8 ;
          wrmsr ;

          ; Enable paging and protected mode
          mov eax, cr0
          or eax, 1 << 31 | 1 << 0 ; Set PG (Paging - bit 31) and PE (Protected Mode - bit 0)
          mov cr0, eax

          ; Jump to long mode
          lgdt [GDT64.Pointer]
          jmp GDT64.Code:long_mode_start

.hang32:
          hlt
          jmp .hang32

          BITS 64
          section   .text

long_mode_start:
          cli
          mov ax, GDT64.Data
          mov ds, ax
          mov es, ax
          mov fs, ax
          mov gs, ax
          mov ss, ax
          mov edi, 0xB8000
          mov rax, 0x1F201F201F201F20
          mov ecx, 500
          rep stosq

.hang:
          hlt
          jmp .hang
