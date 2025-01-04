          bits 32

          %include "return_codes.nasm"

          PRESENT_BIT         equ 1 << 0  ; The page is present in memory
          WRITE_BIT           equ 1 << 1  ; The page is writable
          USER_BIT            equ 1 << 2  ; The page is accessible from user mode
          WRITE_THROUGH_BIT   equ 1 << 3  ; Writes go directly to memory
          CACHE_DISABLE_BIT   equ 1 << 4  ; The page is not cached
          ACCESSED_BIT        equ 1 << 5  ; CPU sets this bit when the page is accessed
          DIRTY_BIT           equ 1 << 6  ; CPU sets this bit when the page is written to
          HUGE_PAGE_BIT       equ 1 << 7  ; Creates a 2 MiB page in P2 table or 1 GiB page in P3 table (must be 0 in P1, P4)
          GLOBAL_BIT          equ 1 << 8  ; The page isn't flushed from the TLB on address space switch
          NO_EXECUTE_BIT      equ 1 << 63 ; Forbid execution from this page

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
          section   .text
setup_page_tables:
          ; Map the first 2 MiB to the first 2 MiB
          ; PML4[0] = PDPT
          ; PDPT[0] = PD
          ; PD[0] = PT

          mov eax, p3_table
          or eax, PRESENT_BIT | WRITE_BIT
          mov [p4_table], eax

          mov eax, p2_table
          or eax, PRESENT_BIT | WRITE_BIT
          mov [p3_table], eax

          ; Identity map the first 2 Mib * 512 = 1 GiB of memory

          ; Setup each entry in p2 table to a page with HUGE_PAGE_BIT set
          ; This will map 2 MiB of memory

          xor ecx, ecx
.map_p2_table:
          mov eax, 0x200000 ; 2 MiB
          mul ecx
          or eax, PRESENT_BIT | WRITE_BIT | HUGE_PAGE_BIT
          mov [p2_table + ecx * 8], eax

          inc ecx
          cmp ecx, 512 ; 512 * 8 = 4096kb = 4mb ; Mapped the whole p2 table
          jl .map_p2_table

          mov eax, SUCCESS_SETUP_PAGE_TABLES
          ret
