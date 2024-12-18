bits 32

section .text32
    global os_hang_32

          ; We expect the kernel to never return from this function.
          ; It is used as a safety net in case something goes wrong.
          ; Additionally current KernelPanic_32 implementation will call this function.
os_hang_32:
    cli
    hlt
    jmp os_hang_32
