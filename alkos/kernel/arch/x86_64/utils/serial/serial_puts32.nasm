          bits 32
          section .text
          global serial_puts
          extern serial_putchar

          NULL equ 0
; void serial_puts(const char* str)
; cdecl calling convention:
; [ebp+8]: pointer to the string

serial_puts:
          push ebp
          mov ebp, esp
          push ebx
          push eax

          mov ebx, [ebp+8]    ; EBX = str

.loop:
          mov al, [ebx]
          cmp al, NULL
          je .done

          push eax
          call serial_putchar
          add esp, 4
          inc ebx             ; Move to next character
          jmp .loop

.done:
          pop eax
          pop ebx
          pop ebp
          ret
