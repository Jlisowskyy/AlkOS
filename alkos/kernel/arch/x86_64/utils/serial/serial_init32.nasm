          bits 32
          COM1 equ 0x3F8
          DLAB_BIT equ 0x80
          BAUD_DIVISOR equ 3

          section .text32
          global serial_init
serial_init:
          push eax
          push edx

          ; Base address: 0x3F8
          ; Disable interrupts: write 0 to IER (COM1+1 = 0x3F9)
          mov dx, COM1+1
          mov al, 0x00
          out dx, al

          ; Set DLAB = 1 in LCR (COM1+3 = 0x3FB)
          mov dx, COM1+3
          mov al, DLAB_BIT
          out dx, al

          ; Set baud rate divisor to 38400 baud (divisor = 3)
          ; DLL (COM1+0 = 0x3F8)
          mov dx, COM1
          mov al, BAUD_DIVISOR
          out dx, al

          ; DLM (COM1+1 = 0x3F9)
          mov dx, COM1+1
          mov al, 0x00
          out dx, al

          ; Clear DLAB and set data bits (8 bits, no parity, one stop bit)
          ; LCR (COM1+3 = 0x3FB)
          mov dx, COM1+3
          mov al, 0x03
          out dx, al

          ; Enable FIFO, clear them, 14-byte threshold: FCR (COM1+2 = 0x3FA)
          mov dx, COM1+2
          mov al, 0xC7
          out dx, al

          ; Enable IRQs, RTS/DSR: MCR (COM1+4 = 0x3FC)
          mov dx, COM1+4
          mov al, 0x0B
          out dx, al

          pop edx
          pop eax
          ret
