; Constants for Multiboot header
MBALIGN   equ  1 << 0              ; align loaded modules on page boundaries
MEMINFO   equ  1 << 1              ; provide memory map
MBFLAGS   equ  MBALIGN | MEMINFO   ; this is the Multiboot 'flag' field
MAGIC     equ  0x1BADB002          ; 'magic number' lets bootloader find the header
CHECKSUM  equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; Multiboot header
section   .multiboot
          align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM
