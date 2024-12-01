          bits 32
; Access bits
PRESENT   equ 1 << 7 ; Present
NOT_SYS   equ 0 << 4 ; Not a system segment
EXEC      equ 1 << 3 ; Executable
DC        equ 1 << 2 ; Direction/Conforming
RW        equ 1 << 1 ; Readable/Writable
AC        equ 1 << 0 ; Accessed
; Flags bits
GRAN_4K   equ 1 << 7 ; 4 KiB granularity
SZ_32     equ 1 << 6 ; 32-bit segment
LONG_MODE equ 1 << 5 ; Long mode

section   .data
          align 16
GDT64:
          .Null:
          dq 0
          .Code: equ $ - GDT64
          dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
          db 0                                        ; Base (mid, bits 16-23)
          db PRESENT | NOT_SYS | EXEC | RW            ; Access
          db GRAN_4K | LONG_MODE | 0xF                ; Flags & Limit (high, bits 16-19)
          db 0                                        ; Base (high, bits 24-31)
          .Data: equ $ - GDT64
          dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
          db 0                                        ; Base (mid, bits 16-23)
          db PRESENT | NOT_SYS | RW                   ; Access
          db GRAN_4K | SZ_32 | 0xF                    ; Flags & Limit (high, bits 16-19)
          db 0                                        ; Base (high, bits 24-31)
          .Pointer:
          dw $ - GDT64 - 1 ; $ - Special symbol that evaluates to the current address
          dq GDT64
.End:

          global GDT64
          global GDT64.Pointer
          global GDT64.Code
          global GDT64.Data
