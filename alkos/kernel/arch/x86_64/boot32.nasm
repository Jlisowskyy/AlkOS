          bits 32

          ; Helper functions
          extern vga_print
          extern framebuffer_print
          extern locate_framebuffer_tag

          ; Serial
          extern serial_init32
          extern serial_print32
          extern serial_puts32

          ; Error checking and handling
          extern check_multiboot
          extern check_cpuid
          extern check_long_mode
          extern handle_return_code

          extern MESSAGE_INIT_ALKOS
          extern MESSAGE_INFO_JUMPING_TO_64

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
boot32:
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
          push      ebx ; Save multiboot_info_t* for later at the top of the stack

          call serial_init32
          push MESSAGE_INIT_ALKOS
          call serial_puts32
          add esp, 4

          call check_multiboot
          call handle_return_code

          ; Use the multiboot information to locate the framebuffer
          call locate_framebuffer_tag
          push eax ; Save multiboot_tag_framebuffer_t* at stack_top - 4

          call check_cpuid
          call handle_return_code
          call check_long_mode
          call handle_return_code

          call setup_page_tables
          call handle_return_code
          call enable_long_mode
          call handle_return_code
          call enable_paging
          call handle_return_code

          push MESSAGE_INFO_JUMPING_TO_64
          call serial_puts32
          add esp, 4

          ; Jump to long mode
          lgdt [GDT64.Pointer]
          jmp GDT64.Code:boot64