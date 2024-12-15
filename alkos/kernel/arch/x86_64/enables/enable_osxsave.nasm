    bits 64

    extern KernelPanic

FEATURE_FLAG equ 0x1      ; CPUID EAX code
XSAVE_FLAG   equ 1<<26    ; ECX

; OSXSAVE Control Register flags

    section .text
    global enable_osxsave

    section .rodata
    FAIL_OSXSAVE db "Missing OSXSAVE feature set. Unable to start...", 0

; Intel manual page 319 (Chapter 13 - Managing State Using the XSAVE Feature Set)
enable_osxsave:
    mov eax, FEATURE_FLAG
    cpuid

    ; check if the OS supports XSAVE
    test ecx, XSAVE_FLAG
    jz enable_osxsave_fail

    ret

enable_osxsave_fail:
    lea rdi, [FAIL_OSXSAVE]
    call KernelPanic
    ret