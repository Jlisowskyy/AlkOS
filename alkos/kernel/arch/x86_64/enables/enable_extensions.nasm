    bits 64

    section .text
    global enable_extensions

; CPUID flags
FEATURE_FLAG equ 0x1      ; CPUID EAX code
SSE_FLAG     equ 1<<25    ; EDX
AVX_FLAG     equ 1<<28    ; ECX
XSAVE_FLAG   equ 1<<26    ; ECX
FXSAVE_FLAG  equ 1<<24    ; EDX

; SSE Control Register flags
OSFXSR_FLAG     equ 1<<9   ; CR4.OSFXSR
OSXMMEXCPT_FLAG equ 1<<10  ; CR4.OSXMMEXCPT
EM_FLAG         equ 1<<2   ; CR0.EM
MP_FLAG         equ 1<<1   ; CR0.MP

; AVX Control Register flags

; According to intel manual page: 3570 (15.1.3 - Initialization of the SSE Extensions)
; TODO:
; 1. SSE CONTEXT SWITCHING
; 2. SSE EXCEPTION HANDLING
; 3. SSE BEHAVIOR
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

    ; step 7
    ; configure the MXCSR register
    ; TODO: IMPLEMENT TOGETHER WITH HANDLERS ETC

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

    ; One of below is expected to be supported to provide context switching for SSE registers

    ; check if FXSAVE and FXRSTOR instructions are supported
    test rdx, FXSAVE_FLAG
    jnz enable_extensions_sse

    ; check if XSAVE and XRSTOR instructions are supported
    test rcx, XSAVE_FLAG
    jz enable_extensions_end

enable_extensions_sse:
    call enable_sse

    ; check if the AVX flag is set
    test rcx, AVX_FLAG
    jz enable_extensions_end
    call enable_avx

enable_extensions_end:
    ret
