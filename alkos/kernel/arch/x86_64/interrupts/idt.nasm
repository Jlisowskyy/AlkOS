; ------------------------------
; Unsupported ISR
; ------------------------------

%macro unsupported_isr 1
isr_wrapper_%+%1:
    mov edi, %1
    call DefaultExceptionHandler
    iretq
%endmacro

; ------------------------------------
; ISR with full gpr save/restore
; ------------------------------------

; register offsets on stack when saving the state
_rax equ 0
_rcx equ 8
_rdx equ 16
_rsi equ 24
_rdi equ 32
_r8  equ 40
_r9  equ 48
_r10 equ 56
_r11 equ 64
_rbp equ 72

; size needed to save the registers on stack
_reg_size equ 8*10

; shadow space needed for C++ functions
_shadow_space equ 8*4

; full isr stack size needed
_isr_stack_size equ _shadow_space + _reg_size

; isr stack frame full regs offset without error
_isr_stack_frame_full_regs_no_err equ 14 + 4

; isr stack frame full regs offset with error
_isr_stack_frame_full_regs_err equ _isr_stack_frame_full_regs_no_err + 1

; We save only the registers which state is volatile in sysV ABI
%macro push_regs 0
    mov qword ptr [rsp + _rax], rax
    mov qword ptr [rsp + _rcx], rcx
    mov qword ptr [rsp + _rdx], rdx
    mov qword ptr [rsp + _rsi], rsi
    mov qword ptr [rsp + _rdi], rdi
    mov qword ptr [rsp + _r8], r8
    mov qword ptr [rsp + _r9], r9
    mov qword ptr [rsp + _r10], r10
    mov qword ptr [rsp + _r11], r11
    mov qword ptr [rsp + _rbp], rbp
%endmacro

%macro pop_regs 0
    mov rax, qword ptr [rsp + _rax]
    mov rcx, qword ptr [rsp + _rcx]
    mov rdx, qword ptr [rsp + _rdx]
    mov rsi, qword ptr [rsp + _rsi]
    mov rdi, qword ptr [rsp + _rdi]
    mov r8, qword ptr [rsp + _r8]
    mov r9, qword ptr [rsp + _r9]
    mov r10, qword ptr [rsp + _r10]
    mov r11, qword ptr [rsp + _r11]
    mov rbp, qword ptr [rsp + _rbp]
%endmacro

; Usual ISR wrappers used to save state and invoke relevant handlers
%macro isr_wrapper_save_general_regs 2
extern isr_%+%1

isr_wrapper_%+%1:
    sub rsp, _reg_size ; create space for the registers
    push_regs

    sub rsp, _shadow_space ; create space for shadow space
    cld

    lea edi, [rsp + 8*%2]
    call isr_%+%1

    add rsp, _shadow_space ; free the shadow space

    pop_regs
    add rsp, _reg_size ; free registers space

    iretq
%endmacro

%macro isr_wrapper_save_general_regs_no_err 1
    isr_wrapper_save_general_regs %1 _isr_stack_frame_full_regs_no_err
%endmacro

%macro isr_wrapper_save_general_regs_err 1
    isr_wrapper_save_general_regs %1 _isr_stack_frame_full_regs_err
%endmacro

; --------------------------------------
; ISR with full state save/restore
; --------------------------------------

%macro isr_wrapper_save_all_regs 1
    ; TODO
    unsupported_isr %1
%endmacro

; ---------------------------------
; ISR without any state saved
; ---------------------------------

%macro isr_wrapper_no_save 1
    ; TODO
    unsupported_isr %1
%endmacro

; ------------------------------
; ISR wrappers definitions
; ------------------------------

bits 64

extern DefaultExceptionHandler
extern LogExceptionReceived

section .text

; Intel defined interrupts
unsupported_isr 0  ; Division Error: Divide by zero error
unsupported_isr 1  ; Debug: Reserved for debugging exceptions
unsupported_isr 2  ; Non-Maskable Interrupt: Non-maskable interrupt detected
unsupported_isr 3  ; Breakpoint: Breakpoint detected
unsupported_isr 4  ; Overflow: Overflow detected
unsupported_isr 5  ; Bound Range Exceeded: Bound range exceeded
unsupported_isr 6  ; Invalid Opcode: Invalid instruction
unsupported_isr 7  ; Device Not Available: FPU device unavailable
unsupported_isr 8  ; Double Fault: Critical CPU error
unsupported_isr 9  ; Coprocessor Segment Overrun: Legacy interrupt ; NOT USED
unsupported_isr 10 ; Invalid TSS: Invalid Task State Segment
unsupported_isr 11 ; Segment Not Present: Segment not present in memory
unsupported_isr 12 ; Stack Segment Fault: Stack-related fault
unsupported_isr 13 ; General Protection Fault: Memory access violation
unsupported_isr 14 ; Page Fault: Page not found in memory
unsupported_isr 15 ; Reserved: Reserved by Intel
unsupported_isr 16 ; x87 Floating-Point Exception: FPU error
unsupported_isr 17 ; Alignment Check: Alignment error
unsupported_isr 18 ; Machine Check: Hardware failure detected
unsupported_isr 19 ; SIMD Floating-Point Exception: SIMD operation error
unsupported_isr 20 ; Virtualization Exception: Virtualization error
unsupported_isr 21 ; Control Protection Exception
unsupported_isr 22 ; Reserved: Reserved by Intel
unsupported_isr 23 ; Reserved: Reserved by Intel
unsupported_isr 24 ; Reserved: Reserved by Intel
unsupported_isr 25 ; Reserved: Reserved by Intel
unsupported_isr 26 ; Reserved: Reserved by Intel
unsupported_isr 27 ; Reserved: Reserved by Intel
unsupported_isr 28 ; Hypervisor Injection Exception
unsupported_isr 29 ; VMM Communication Exception
unsupported_isr 30 ; Security Exception: Security-related error
unsupported_isr 31 ; Reserved: Reserved by Intel

; IRQs for PICs
unsupported_isr 32 ; IRQ0: System timer
unsupported_isr 33 ; IRQ1: Keyboard
unsupported_isr 34 ; IRQ2: Cascade (used by second PIC) this will (should) never come
unsupported_isr 35 ; IRQ3: Serial port 2
unsupported_isr 36 ; IRQ4: Serial port 1
unsupported_isr 37 ; IRQ5: Parallel port 2 or sound card
unsupported_isr 38 ; IRQ6: Floppy disk controller
unsupported_isr 39 ; IRQ7: Parallel port 1
unsupported_isr 40 ; IRQ8: Real-time clock
unsupported_isr 41 ; IRQ9: Free for peripherals / legacy SCSI / NIC
unsupported_isr 42 ; IRQ10: Free for peripherals / SCSI / NIC
unsupported_isr 43 ; IRQ11: Free for peripherals / SCSI / NIC
unsupported_isr 44 ; IRQ12: Mouse
unsupported_isr 45 ; IRQ13: FPU (legacy)
unsupported_isr 46 ; IRQ14: Primary ATA channel
unsupported_isr 47 ; IRQ15: Secondary ATA channel

; ----------------------------------
; ISR wrapper table definition
; ----------------------------------

section .data

global IsrWrapperTable
IsrWrapperTable:
%assign i 0
%rep    48
    dq isr_wrapper_%+i
%assign i i+1
%endrep
