    bits 32

    section .text32
    global TerminalInit_32
    global TerminalPutChar_32
    global TerminalWriteString_32
    global TerminalWriteError_32

    extern serial_init32
    extern serial_putchar32
    extern serial_puts32
    extern vga_print

TerminalInit_32:
%ifdef __DEBUG_TERMINAL_TEST__
    call serial_init32
%endif

    ; No need to initialize VGA here
    ret

TerminalPutChar_32:
    push ebp
    mov ebp, esp

%ifdef __DEBUG_TERMINAL_TEST__
    push dword [ebp + 8]
    call serial_putchar32
    add esp, 4
%endif

    ; create a string from the character
    push byte 0
    sub esp, 1
    mov eax, [ebp + 8]
    mov byte [esp], al
    push esp
    call vga_print
    add esp, 6

    leave
    ret

TerminalWriteString_32:
    push ebp
    mov ebp, esp

%ifdef __DEBUG_TERMINAL_TEST__
    push dword [ebp + 8]
    call serial_puts32
    add esp, 4
%endif

    ; Print the string to the VGA terminal
    push dword [ebp + 8]
    call vga_print
    add esp, 4

    leave
    ret

TerminalWriteError_32:
    push ebp
    mov ebp, esp

    ; TODO: Implement different print color for errors
    push dword [ebp + 8]
    call TerminalWriteString_32
    add esp, 4

    leave
    ret

