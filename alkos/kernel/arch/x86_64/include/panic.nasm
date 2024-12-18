extern KernelPanic_32
extern KernelPanic
extern os_hang
extern os_hang_32

; Panic is a function that will print an error message and halt the system.

%macro panic_32 1
    push %1
    call KernelPanic_32
%endmacro

%macro panic_64 1
    mov rdi, %1
    call KernelPanic
%endmacro
