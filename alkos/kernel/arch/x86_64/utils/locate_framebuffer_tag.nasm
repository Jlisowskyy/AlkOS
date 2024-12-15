          bits 32
          %include "return_codes.nasm"

MULTIBOOT_TAG_TYPE_FRAMEBUFFER equ 8

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

          mov eax, [ebp + 8] ; eax = multiboot_info_t*
          mov ebx, [eax]     ; total_size = mbi->total_size
          mov esi, eax       ; mbi start  = mbi
          add esi, ebx       ; mbi end    = mbi + total_size

          cmp ebx, 8         ; Check if total_size is more than 8 (at least one tag exists)
          jle .no_tags

          add eax, 8         ; Jump to the first tag,

.loop:
          cmp dword [eax], MULTIBOOT_TAG_TYPE_FRAMEBUFFER
          je .end

          add eax, [eax + 4] ; mbi->tag = mbi->tag + mbi->tag->size
          cmp eax, esi       ; Check if we reached the end of the mbi
          jge .no_tags
          jmp .loop

.no_tags:
          mov eax, 0
.end:
          pop edi
          pop esi
          pop ebx
          pop ebp
          ret
