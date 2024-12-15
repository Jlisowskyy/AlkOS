; -----------------------------------------------------------------------------
; File: serial64.asm
; Description: Serial port communication routines for 64-bit systems.
;              Includes initialization, character output, and string output
;              functions for COM1.
; -----------------------------------------------------------------------------
          bits 64

; -----------------------------------------------------------------------------
; Constants Definition
; -----------------------------------------------------------------------------
COM1        equ 0x3F8    ; Base address for COM1
DLAB_BIT    equ 0x80     ; Divisor Latch Access Bit
BAUD_DIVISOR equ 3       ; Divisor for 38400 baud
EMPTY       equ 0x20     ; Line Status Register bit indicating empty
NEWLINE     equ 0x0A     ; Line Feed
SLASH_R     equ 0x0D     ; Carriage Return
NULL        equ 0        ; Null terminator for strings

; -----------------------------------------------------------------------------
; Section: Text (64-bit)
; -----------------------------------------------------------------------------
          section .text
          global serial_init
          global serial_putchar
          global serial_puts

; -----------------------------------------------------------------------------
; Function: serial_init
; Description:
;     Initializes the serial port (COM1) for communication with the following
;     settings:
;         - Baud rate: 38400
;         - Data bits: 8
;         - Parity: None
;         - Stop bits: 1
; Parameters:
;     None
; Returns:
;     void
; -----------------------------------------------------------------------------
serial_init:
          push    rax
          push    rdx

          ; Disable interrupts: write 0 to IER (COM1+1 = 0x3F9)
          mov     dx, COM1 + 1
          mov     al, 0x00
          out     dx, al

          ; Set DLAB = 1 in LCR (COM1+3 = 0x3FB)
          mov     dx, COM1 + 3
          mov     al, DLAB_BIT
          out     dx, al

          ; Set baud rate divisor to 38400 baud (divisor = 3)
          ; DLL (COM1+0 = 0x3F8)
          mov     dx, COM1
          mov     al, BAUD_DIVISOR
          out     dx, al

          ; DLM (COM1+1 = 0x3F9)
          mov     dx, COM1 + 1
          mov     al, 0x00
          out     dx, al

          ; Clear DLAB and set data bits (8 bits, no parity, one stop bit)
          ; LCR (COM1+3 = 0x3FB)
          mov     dx, COM1 + 3
          mov     al, 0x03
          out     dx, al

          ; Enable FIFO, clear them, 14-byte threshold: FCR (COM1+2 = 0x3FA)
          mov     dx, COM1 + 2
          mov     al, 0xC7
          out     dx, al

          ; Enable IRQs, RTS/DSR: MCR (COM1+4 = 0x3FC)
          mov     dx, COM1 + 4
          mov     al, 0x0B
          out     dx, al

          ; Restore RDX and RAX registers
          pop     rdx
          pop     rax
          ret

; -----------------------------------------------------------------------------
; Function: serial_putchar
; Description:
;     Sends a single character to the serial port (COM1).
; Parameters:
;     char c  - The character to send. Passed in the lower 8 bits of RDI.
; Returns:
;     void
; -----------------------------------------------------------------------------
serial_putchar:
          push    rbp
          mov     rbp, rsp
          push    rax
          push    rbx
          push    rdx

          ; Move the character from RDI (first argument) to AL
          mov     al, dil

          ; Wait until the transmit buffer is empty
.wait_ready:
          ; Read Line Status Register (LSR) at COM1+5 = 0x3FD
          mov     dx, COM1 + 5
          in      al, dx
          test    al, EMPTY
          jz      .wait_ready

          ; Send the character to COM1
          mov     al, dil
          mov     dx, COM1
          out     dx, al

          pop     rdx
          pop     rbx
          pop     rax
          pop     rbp
          ret

; -----------------------------------------------------------------------------
; Function: serial_puts
; Description:
;     Sends a null-terminated string to the serial port (COM1).
; Parameters:
;     const char* str  - Pointer to the null-terminated string. Passed as
;                        the first argument in RDI.
; Returns:
;     void
; -----------------------------------------------------------------------------
serial_puts:
          push    rbp
          mov     rbp, rsp
          push    rbx
          push    rax

          ; Move the string pointer from RDI (first argument) to RBX
          mov     rbx, rdi

.loop:
          ; Load the current character into AL
          mov     al, byte [rbx]
          cmp     al, NULL
          je      .done

          ; Move the character to DIL (lower 8 bits of RDI) for serial_putchar
          mov     dil, al
          call    serial_putchar

          ; Move to the next character in the string
          inc     rbx
          jmp     .loop

.done:
          pop     rax
          pop     rbx
          pop     rbp
          ret
