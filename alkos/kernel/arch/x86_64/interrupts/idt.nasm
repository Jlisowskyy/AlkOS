; Scheme copied from osdev.org
; Simplifies definition of idt

; ISR stubs terminating kernel when received
%macro isr_err_stub 1
isr_stub_%+%1:
    mov edi, %1
    call DefaultExceptionHandler
    iretq
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
    mov edi, %1
    call DefaultExceptionHandler
    iretq
%endmacro

; Usual ISR wrappers used to save state and invoke relevant handlers
%macro isr_wrapper_save_general_regs 1
isr_wrapper_%+%1:
    ; TODO
    mov edi, %1
    call LogExceptionReceived
    iretq
%endmacro

%macro isr_wrapper_save_all_regs 1
    ; TODO
    mov edi, %1
    call LogExceptionReceived
    iretq
%endmacro

%macro isr_wrapper_no_save 1
    ; TODO
    mov edi, %1
    call DefaultExceptionHandler
    iretq
%endmacro

bits 64

extern DefaultExceptionHandler
extern LogExceptionReceived

section .text

; Intel defined interrupts
isr_no_err_stub 0  ; Division Error: Divide by zero error
isr_no_err_stub 1  ; Debug: Reserved for debugging exceptions
isr_no_err_stub 2  ; Non-Maskable Interrupt: Non-maskable interrupt detected
isr_no_err_stub 3  ; Breakpoint: Breakpoint detected
isr_no_err_stub 4  ; Overflow: Overflow detected
isr_no_err_stub 5  ; Bound Range Exceeded: Bound range exceeded
isr_no_err_stub 6  ; Invalid Opcode: Invalid instruction
isr_no_err_stub 7  ; Device Not Available: FPU device unavailable
isr_err_stub    8  ; Double Fault: Critical CPU error
isr_no_err_stub 9  ; Coprocessor Segment Overrun: Legacy interrupt ; NOT USED
isr_err_stub    10 ; Invalid TSS: Invalid Task State Segment
isr_err_stub    11 ; Segment Not Present: Segment not present in memory
isr_err_stub    12 ; Stack Segment Fault: Stack-related fault
isr_err_stub    13 ; General Protection Fault: Memory access violation
isr_err_stub    14 ; Page Fault: Page not found in memory
isr_no_err_stub 15 ; Reserved: Reserved by Intel
isr_no_err_stub 16 ; x87 Floating-Point Exception: FPU error
isr_err_stub    17 ; Alignment Check: Alignment error
isr_no_err_stub 18 ; Machine Check: Hardware failure detected
isr_no_err_stub 19 ; SIMD Floating-Point Exception: SIMD operation error
isr_no_err_stub 20 ; Virtualization Exception: Virtualization error
isr_no_err_stub 21 ; Control Protection Exception
isr_no_err_stub 22 ; Reserved: Reserved by Intel
isr_no_err_stub 23 ; Reserved: Reserved by Intel
isr_no_err_stub 24 ; Reserved: Reserved by Intel
isr_no_err_stub 25 ; Reserved: Reserved by Intel
isr_no_err_stub 26 ; Reserved: Reserved by Intel
isr_no_err_stub 27 ; Reserved: Reserved by Intel
isr_no_err_stub 28 ; Hypervisor Injection Exception
isr_no_err_stub 29 ; VMM Communication Exception
isr_err_stub    30 ; Security Exception: Security-related error
isr_no_err_stub 31 ; Reserved: Reserved by Intel

; IRQs for PICs
isr_no_err_stub 32 ; IRQ0: System timer
isr_no_err_stub 33 ; IRQ1: Keyboard
isr_no_err_stub 34 ; IRQ2: Cascade (used by second PIC) this will (should) never come
isr_no_err_stub 35 ; IRQ3: Serial port 2
isr_no_err_stub 36 ; IRQ4: Serial port 1
isr_no_err_stub 37 ; IRQ5: Parallel port 2 or sound card
isr_no_err_stub 38 ; IRQ6: Floppy disk controller
isr_no_err_stub 39 ; IRQ7: Parallel port 1
isr_no_err_stub 40 ; IRQ8: Real-time clock
isr_no_err_stub 41 ; IRQ9: Free for peripherals / legacy SCSI / NIC
isr_no_err_stub 42 ; IRQ10: Free for peripherals / SCSI / NIC
isr_no_err_stub 43 ; IRQ11: Free for peripherals / SCSI / NIC
isr_no_err_stub 44 ; IRQ12: Mouse
isr_no_err_stub 45 ; IRQ13: FPU (legacy)
isr_no_err_stub 46 ; IRQ14: Primary ATA channel
isr_no_err_stub 47 ; IRQ15: Secondary ATA channel

section .data

global IsrStubTable
IsrStubTable:
%assign i 0
%rep    48
    dq isr_stub_%+i
%assign i i+1
%endrep
