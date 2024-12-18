extern TerminalWriteString_32
extern TerminalWriteString

; Macros simplifying logging for kernel written in NASM
%macro trace_32 1
    push %1
    call TerminalWriteString_32
    add esp, 4
%endmacro

%macro trace_64 1
    mov rdi, %1
    call TerminalWriteString
%endmacro