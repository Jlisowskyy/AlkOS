extern KernelPanic_32
extern KernelPanic
extern os_hang_32
extern os_hang
extern MESSAGE_ERROR_KERNEL_PANIC_RETURNED

%include "log.nasm"
; Panic is a function that will print an error message and halt the system.

%macro panic_32 1
    sub esp, 12
    push %1
    call KernelPanic_32
    add esp, 16

    trace_32 MESSAGE_ERROR_KERNEL_PANIC_RETURNED

    jmp os_hang_32
%endmacro

%macro panic_64 1
    mov rdi, %1
    call KernelPanic
%endmacro
