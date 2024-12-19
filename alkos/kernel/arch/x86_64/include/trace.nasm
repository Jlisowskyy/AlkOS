extern serial_puts32
extern serial_puts

%macro trace_32 1
    push %1
    call serial_puts32
    add esp, 4
%endmacro

%macro trace 1
    push rdi
    mov rdi, %1
    call serial_puts
    pop rdi
%endmacro
