          bits 32

          FRAMEBUFFER_INFO_TAG_POINTER equ stack_top - 8
          FRAMEBUFFER_ADDR equ FRAMEBUFFER_INFO_TAG_POINTER + 8
          FRAMEBUFFER_PITCH equ FRAMEBUFFER_INFO_TAG_POINTER + 16
          FRAMEBUFFER_WIDTH equ FRAMEBUFFER_INFO_TAG_POINTER + 20
          FRAMEBUFFER_HEIGHT equ FRAMEBUFFER_INFO_TAG_POINTER + 24
          FRAMEBUFFER_BPP equ FRAMEBUFFER_INFO_TAG_POINTER + 26
          FRAMEBUFFER_TYPE equ FRAMEBUFFER_INFO_TAG_POINTER + 28

          extern stack_top
          extern reg_to_message

          section .text
          global framebuffer_print
framebuffer_print:
          push ebp
          mov ebp, esp
          push ebx
          push esi
          push edi

          mov esi, [ebp + 8]          ; string ptr
          mov ebx, [FRAMEBUFFER_PITCH]
          mov ax, [FRAMEBUFFER_BPP]
          cmp ax, 32
          jne .finish
          mov al, [FRAMEBUFFER_TYPE]
          cmp al, 1
          jne .finish

          xor edx, edx ; char_x=0
          xor ecx, ecx ; char_y=0

.print_loop:
          mov al, [esi]
          cmp al, 0
          je .finish
          inc esi

          cmp al, 0x0A
          je .line_break

          cmp al, 0x20
          jb .skip_render
          cmp al, 0x7E
          ja .skip_render

          ; pixel_x = char_x*8 in ESI
          ; pixel_y = char_y*8 in EDI
          mov esi, edx
          shl esi, 3
          mov edi, ecx
          shl edi, 3

          ; Draw an 8x8 block of white pixels:
          mov eax, 0xFFFFFFFF ; white

          ; Rows in ECX:
          push edx
          push ecx
          xor ecx, ecx ; row=0
.draw_rows_clean:
          cmp ecx, 8
          jge .done_char
          ; row_address = framebuffer_addr + ((pixel_y+row)*pitch)
          mov ebp, edi
          add ebp, ecx
          mov edx, [FRAMEBUFFER_PITCH]
          imul ebp, edx
          mov edx, [FRAMEBUFFER_ADDR]
          add ebp, edx ; ebp = row start address

          ; Columns:
          xor edx, edx ; col=0
.draw_cols_clean:
          cmp edx, 8
          jge .next_row
          ; pixel offset = (pixel_x+col)*4
          mov ebx, esi
          add ebx, edx
          shl ebx, 2

          ; write white pixel:
          mov [ebp+ebx], eax

          inc edx
          jmp .draw_cols_clean

.next_row:
          inc ecx
          jmp .draw_rows_clean

.done_char:
          pop ecx
          pop edx

.skip_render:
          inc edx ; advance char_x
          jmp .print_next

.line_break:
          inc ecx  ; char_y++
          xor edx, edx ; char_x=0
          jmp .print_next

.print_next:
          jmp .print_loop

.finish:
          pop edi
          pop esi
          pop ebx
          pop ebp
          ret
