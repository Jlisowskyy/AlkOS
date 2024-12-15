          bits 32
          COM1 equ 0x3F8
          EMPTY equ 0x20
          NEWLINE equ 0x0A
          SLASH_R equ 0x0D
          SLASH_N equ 0x0A

          section .text32
          global serial_putchar
          ; void serial_print(char c)
          ; [ebp+8]: character c
serial_putchar:

          push ebp
          mov ebp, esp
          push eax
          push ebx
          push edx

          mov al, [ebp+8]        ; Get character from stack

          mov dx, COM1

.wait_ready:
          ; Read LSR at COM1+5 = 0x3FD
          mov dx, COM1+5
          in al, dx
          test al, EMPTY          ; THR Empty bit
          jz .wait_ready

          mov al, [ebp+8]
          mov dx, COM1
          out dx, al             ; Send character

          pop edx
          pop ebx
          pop eax
          pop ebp
          ret
