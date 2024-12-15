    bits 64

    section .text
    global enable_extensions

; CPUID flags
FEATURE_FLAG equ 0x1
SSE_FLAG     equ 1<<25
AVX_FLAG     equ 1<<28

; SSE Control Register flags
OSFXSR_FLAG     equ 1<<9
OSXMMEXCPT_FLAG equ 1<<10
EM_FLAG         equ 1<<2
MP_FLAG         equ 1<<1

; AVX Control Register flags

; According to intel manual page: 3570 (15.1.3 - Initialization of the SSE Extensions)
enable_sse:
    mov rax, cr4

    ; step 1
    ; set CR4.OSFXSR[bit 9] to 1 - explicitly stating that os supports context switching of SSE registers
    ; TODO: STATED NOT IMPLEMENTED
    or rax, OSFXSR_FLAG

    ; step 2
    ; set CR4.OSXMMEXCPT[bit 10] to 1 - explicitly stating that os supports SSE exceptions by providing an SSE exception handler
    ; TODO: STATED NOT IMPLEMENTED
    or rax, OSXMMEXCPT_FLAG

    ; step 3
    ; write back to CR4 and load CR0
    mov cr4, rax
    mov rax, cr0

    ; step 4
    ; set CR0.EM[bit 2] to 0 - disable x87 FPU emulation - required for SSE
    and rax, ~EM_FLAG

    ; step 5
    ; set CR0.MP[bit 1] to 1 - referenced to section 11.2.1 - not stated but required for SSE
    or rax, MP_FLAG

    ; step 6
    ; write back to CR0
    mov cr0, rax

    ret

enable_avx:
    ret

enable_extensions:
    ; query the cpu for supported features
    mov rax, FEATURE_FLAG
    cpuid

    ; check if the SSE flag is set
    test rdx, SSE_FLAG
    jz enable_extensions_end
    call enable_sse

    ; check if the AVX flag is set
    test rcx, AVX_FLAG
    jz enable_extensions_end
    call enable_avx

enable_extensions_end:
    ret
