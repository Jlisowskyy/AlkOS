          global stack_bottom
          global stack_top

          section .data
          align 8
stack_bottom:
          resb 16384
stack_top: