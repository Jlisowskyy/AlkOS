          bits 32

vga_base  equ 0xB8000
vga_width equ 80
vga_height equ 25
vga_attr  equ 0x07 ; white on black

          section .text32
          global vga_print
vga_print:
          ; void vga_print(const char *message);
          ; Print a message to the screen using VGA text mode.
          ; Note: Does not handle newlines.
          ; Inputs:
          ;   EAX = pointer to the message
          ; Outputs:
          ;   None
begin:
          push ebp
          mov ebp, esp

          ; Save the registers we will be using
          push ebx
          push esi
          push edi

          ; Get the message pointer
          mov esi, [ebp + 8]

          ; Get the VGA base address
          mov edi, vga_base

          ; Loop through the message
.loop:
          mov al, [esi]

          ; Check for the end of the string
          test al, al
          jz .end
.print_char:
          mov ah, vga_attr ; set the attribute byte (foreground / background)
                         ; al is already set to the character we want to print
          mov [edi], ax  ; Write the character
          add edi, 2     ; Move to the next character
          inc esi        ; Move to the next character
          jmp .loop
.end:
          ; Restore the registers we used
          pop edi
          pop esi
          pop ebx

          ; Restore the stack and return
          mov esp, ebp
          pop ebp
          ret

