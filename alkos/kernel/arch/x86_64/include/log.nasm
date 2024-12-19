extern TerminalWriteString_32
extern TerminalWriteString
extern serial_puts_32

; Macros simplifying logging for kernel written in NASM
%macro trace_32 1
    sub esp, 12
    push %1
    call TerminalWriteString_32
    add esp, 16
%endmacro

%macro trace_64 1
    mov rdi, %1
    call TerminalWriteString
%endmacro