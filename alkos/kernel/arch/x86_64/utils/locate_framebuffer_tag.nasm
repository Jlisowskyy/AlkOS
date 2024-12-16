          bits 32
          %include "return_codes.nasm"

          MULTIBOOT_TAG_TYPE_FRAMEBUFFER equ 8

          TAB equ 0x09
          SPACE equ 0x20
          NEWLINE equ 0x0A

          extern serial_puts32
          extern reg_to_message

          section .rodata
          MESSAGE_NEW_TAG db "[INFO]", TAB, SPACE, SPACE, "New tag: ", NEWLINE, 0
          MESSAGE_TAG_NOT_FOUND db "[WARN]", TAB, SPACE, SPACE, "Tag not found", NEWLINE, 0

          section .text32
          ; struct multiboot_tag_framebuffer
          ;         +--------------------+
          ; u32     | type = 8           |
          ; u32     | size               |
          ; u64     | framebuffer_addr   |
          ; u32     | framebuffer_pitch  |
          ; u32     | framebuffer_width  |
          ; u32     | framebuffer_height |
          ; u8      | framebuffer_bpp    |
          ; u8      | framebuffer_type   |
          ; u8      | reserved           |
          ; varies  | color_info         |
          ;         +--------------------+

          ; struct multiboot_info_t
          ;         +-------------------+
          ; u32     | total_size        |
          ; u32     | reserved          |
          ;         +-------------------+
          ;         | tags              |
          ;         | ...               |
          ;         +-------------------+
          global locate_framebuffer_tag
          ; multiboot_tag_framebuffer_t* locate_framebuffer_tag(multiboot_info_t* mbi);
          ;                                                                  [ebp + 8]
locate_framebuffer_tag:
          push ebp
          mov ebp, esp

          push ebx
          push esi
          push edi
          push ecx
          push edx

          mov edi, [ebp + 8] ; edi = multiboot_info_t*
          mov ebx, edi       ; mbi start  = mbi
          add ebx, [edi]     ; mbi end    = mbi + mbi->total_size
          push ebx           ; Save mbi end

          cmp ebx, 8         ; Check if total_size is more than 8 (at least one tag exists)
          jle .no_tags

          add edi, 8         ; Jump to the first tag,

.loop:
          ; Debug prints for the tag type and size
          push MESSAGE_NEW_TAG
          call serial_puts32
          add esp, 4

          push dword [edi]
          call reg_to_message
          add esp, 4

          push eax
          call serial_puts32
          add esp, 4

          push dword [edi + 4]
          call reg_to_message
          add esp, 4

          push eax
          call serial_puts32
          add esp, 4

          ; Check if the tag is a framebuffer tag
          cmp dword [edi], MULTIBOOT_TAG_TYPE_FRAMEBUFFER
          je .end

          ; Align the tag size to 8 bytes
          xor edx, edx        ; Clear edx (is old part of extended accumulator and used for division)
          mov ecx, 8          ; 8 bytes alignment
          div ecx             ; Remainder in edx
          mov eax, [edi + 4]  ; load tag size
          cmp edx, 0          ; If remainder is 0 - tag size is already aligned
          je .align8
.not_aligned:
          add eax, ecx        ; tag size + 8
          sub eax, edx        ; tag size + 8 - remainder
.align8:
          add edi, eax        ; add aligned tag size

          ; Debug prints for aligned tag size
          push eax
          call reg_to_message
          add esp, 4

          push eax
          call serial_puts32
          add esp, 4

          ; Check if we reached the end of the multiboot tags
          cmp edi, [esp]
          jge .no_tags
          jmp .loop
.no_tags:
          push MESSAGE_TAG_NOT_FOUND
          call serial_puts32
          add esp, 4

          mov eax, 0 ; NULL on failure
.end:
          mov eax, edi ; multiboot_tag_framebuffer_t* on success

          add esp, 4
          pop edx
          pop ecx
          pop edi
          pop esi
          pop ebx
          pop ebp
          ret
