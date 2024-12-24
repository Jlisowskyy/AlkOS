extern serial_puts32
extern QemuTerminalWriteString

%ifdef __DEBUG_TERMINAL_TEST__

%macro trace_32 1
    push %1
    call serial_puts32
    add esp, 4
%endmacro

%macro trace 1
    push rdi
    mov rdi, %1
    call QemuTerminalWriteString
    pop rdi
%endmacro

%else

%macro trace_32 1
%endmacro

%macro trace 1
%endmacro

%endif



