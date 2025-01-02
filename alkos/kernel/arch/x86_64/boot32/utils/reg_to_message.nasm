          bits 32

          NEWLINE equ 0x0A

          section .bss
          MESSAGE_BUFFER resb 256

          section .text32
          global reg_to_message
          ; void reg_to_message(uint32_t value);
          ; Convert a register value to corresponding ASCII characters and store them in MESSAGE_BUFFER with a newline
          ; and null terminator.
reg_to_message:
          push ebp
          mov ebp, esp
          push ebx
          push ecx
          push edx
          push esi

          mov eax, [ebp + 8]          ; EAX = input value
          mov esi, MESSAGE_BUFFER

          ; Check if the input value is zero
          cmp eax, 0
          jne .convert_number
          mov byte [esi], '0'         ; If zero, store '0'
          inc esi
          jmp .append_newline

.convert_number:
          mov ebx, eax                ; EBX = copy of the input value
          mov ecx, 0                  ; ECX = digit count

.convert_loop:
          xor edx, edx                ; Clear EDX before division
          mov eax, ebx                ; Move current value to EAX
          mov ebx, 10                 ; Divisor = 10
          div ebx                     ; EAX = EAX / 10, EDX = EAX % 10 (remainder)
          add dl, '0'                 ; Convert remainder to ASCII
          push edx                    ; Push the ASCII digit onto the stack
          inc ecx                     ; Increment digit count
          mov ebx, eax                ; Update EBX with the quotient
          cmp eax, 0
          jne .convert_loop           ; Continue if quotient != 0

          ; Pop the digits from the stack and store them in MESSAGE_BUFFER
          mov eax, ecx                ; Number of digits
          mov ecx, eax                ; Set ECX as loop counter

.pop_digits:
          pop edx                     ; Retrieve the top digit
          mov [esi], dl               ; Store the digit in the buffer
          inc esi                     ; Move to the next buffer position
          loop .pop_digits            ; Repeat for all digits

.append_newline:
          mov byte [esi], NEWLINE     ; Append newline character
          inc esi
          mov byte [esi], 0           ; Append null terminator

          mov eax, MESSAGE_BUFFER

          pop esi
          pop edx
          pop ecx
          pop ebx
          mov esp, ebp
          pop ebp
          ret
