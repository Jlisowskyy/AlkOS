#include <idt.hpp>

extern "C" {

// ------------------------------
// IRQ0: System timer
// ------------------------------

void isr_32(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 32);
}

// ------------------------------
// IRQ1: keyboard
// ------------------------------

void isr_33(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 33);
}

// ------------------------------
// IRQ3: COM2
// ------------------------------

void isr_35(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 35);
}

// ------------------------------
// IRQ4: COM1
// ------------------------------

void isr_36(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 36);
}

// ------------------------------
// IRQ8: real time clock
// ------------------------------

void isr_40(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 40);
}

// ------------------------------
// IRQ12: mouse
// ------------------------------

void isr_44(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 44);
}

// ------------------------------
// Test isr
// ------------------------------

void isr_48(void *const stack_frame)
{
    LogIrqReceived(stack_frame, 48);
}
}
