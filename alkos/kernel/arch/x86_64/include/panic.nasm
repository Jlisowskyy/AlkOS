extern KernelPanic

%macro panic_32 1
    ; TODO: Implement panic for 32-bit mode
%endmacro

%macro panic 1
    mov rdi, %1
    call KernelPanic
%endmacro
