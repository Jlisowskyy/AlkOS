; Constants for Multiboot header
ARCHINFO  equ  0                   ; '0' means 32-bit protected mode of the i386 architecture
MAGIC     equ  0xE85250D6          ; 'magic number' lets bootloader find the header
CHECKSUM  equ -(MAGIC + ARCHINFO + (multiboot_end - multiboot_start))

; Multiboot header
section   .multiboot
          align 8
multiboot_start:
	dd MAGIC
	dd ARCHINFO
	dd multiboot_end - multiboot_start
	dd CHECKSUM

	dw 0
	dw 0
	dd 8
multiboot_end:
