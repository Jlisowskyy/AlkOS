extern TerminalWriteString_32
extern TerminalWriteString

%macro puts_32 1
    push %1
    call TerminalWriteString_32
    add esp, 4
%endmacro

%macro puts 1
    push rdi
    mov rdi, %1
    call TerminalWriteString
    pop rdi
%endmacro
