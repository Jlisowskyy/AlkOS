          bits 32

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
