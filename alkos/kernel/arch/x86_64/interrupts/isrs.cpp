#include <idt.hpp>

extern "C" {

// ------------------------------
// IRQ0: System timer
// ------------------------------

void isr_32(void *const stack_frame) { LogIrqReceived(stack_frame, 32); }

// ------------------------------
// IRQ1: keyboard
// ------------------------------

void isr_33(void *const stack_frame) { LogIrqReceived(stack_frame, 33); }

// ------------------------------
// IRQ3: COM2
// ------------------------------

void isr_35(void *const stack_frame) { LogIrqReceived(stack_frame, 35); }

// ------------------------------
// IRQ4: COM1
// ------------------------------

void isr_36(void *const stack_frame) { LogIrqReceived(stack_frame, 36); }

// ------------------------------
// IRQ8: real time clock
// ------------------------------

void isr_40(void *const stack_frame) { LogIrqReceived(stack_frame, 40); }

// ------------------------------
// IRQ12: mouse
// ------------------------------

void isr_44(void *const stack_frame) { LogIrqReceived(stack_frame, 44); }

// ------------------------------
// Test isr
// ------------------------------

void isr_48(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 48);

    /* pollute all registers possible */
    __asm__ volatile("movq $-1, %%rax" : : : "rax");
    __asm__ volatile("movq $-1, %%rbx" : : : "rbx");
    __asm__ volatile("movq $-1, %%rcx" : : : "rcx");
    __asm__ volatile("movq $-1, %%rdx" : : : "rdx");
    __asm__ volatile("movq $-1, %%rsi" : : : "rsi");
    __asm__ volatile("movq $-1, %%rdi" : : : "rdi");
    __asm__ volatile("movq $-1, %%r8" : : : "r8");
    __asm__ volatile("movq $-1, %%r9" : : : "r9");
    __asm__ volatile("movq $-1, %%r10" : : : "r10");
    __asm__ volatile("movq $-1, %%r11" : : : "r11");
    __asm__ volatile("movq $-1, %%r12" : : : "r12");
    __asm__ volatile("movq $-1, %%r13" : : : "r13");
    __asm__ volatile("movq $-1, %%r14" : : : "r14");
    __asm__ volatile("movq $-1, %%r15" : : : "r15");
}
}
