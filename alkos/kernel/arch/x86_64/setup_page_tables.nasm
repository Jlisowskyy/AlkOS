          bits 32
          %include "error_codes.nasm"

          global p4_table
          global p3_table
          global p2_table
          global p1_table
          section   .bss
          align 4096
p4_table:
          resb 4096 ; Page Map Level 4
p3_table:
          resb 4096 ; Page Directory Pointer Table
p2_table:
          resb 4096 ; Page Directory
p1_table:
          resb 4096 ; Page Table

          global setup_page_tables
          section   .text32
setup_page_tables:
          ; Map the first 2 MiB to the first 2 MiB
          ; PML4[0] = PDPT
          ; PDPT[0] = PD
          ; PD[0] = PT

          mov eax, p3_table
          or eax, 0x3 ; Present, Read/Write
          mov [p4_table], eax

          mov eax, p2_table
          or eax, 0x3 ; Present, Read/Write
          mov [p3_table], eax

          mov eax, p1_table
          or eax, 0x3 ; Present, Read/Write
          mov [p2_table], eax

          ; Map the first 2 MiB to the first 2 MiB
          mov edi, p1_table
          mov ebx, 0x00000003
          mov ecx, 512

.map_p1_table:
          mov dword [edi], ebx
          add ebx, 4096
          add edi, 8
          loop .map_p1_table

          mov al, NO_ERROR
          ret
