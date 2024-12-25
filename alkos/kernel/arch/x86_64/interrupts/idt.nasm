; Scheme copied from osdev.org
; Simplifies definition of idt

%macro unsupported_isr 1
isr_wrapper_%+%1:
    mov edi, %1
    call DefaultExceptionHandler
    iretq
%endmacro

; Usual ISR wrappers used to save state and invoke relevant handlers
%macro isr_wrapper_save_general_regs 1
isr_wrapper_%+%1:
    ; TODO
    mov edi, %1
    call isr_%+%1
    iretq
%endmacro

%macro isr_wrapper_save_all_regs 1
    ; TODO
    mov edi, %1
    call isr_%+%1
    iretq
%endmacro

%macro isr_wrapper_no_save 1
    ; TODO
    mov edi, %1
    call isr_%+%1
    iretq
%endmacro

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

section .data

global IsrStubTable
IsrStubTable:
%assign i 0
%rep    48
    dq isr_wrapper_%+i
%assign i i+1
%endrep
