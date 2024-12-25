#include <idt.hpp>

extern "C" {

// ------------------------------
// IRQ0: System timer
// ------------------------------

void isr_32(void *const stack_frame, const u8 exception_code)
{
    LogIrqReceived(stack_frame, exception_code);
}

// ------------------------------
// IRQ1: keyboard
// ------------------------------

void isr_33(void *const stack_frame, const u8 exception_code)
{
    LogIrqReceived(stack_frame, exception_code);
}

// ------------------------------
// IRQ3: COM2
// ------------------------------

void isr_35(void *const stack_frame, const u8 exception_code)
{
    LogIrqReceived(stack_frame, exception_code);
}

// ------------------------------
// IRQ4: COM1
// ------------------------------

void isr_36(void *const stack_frame, const u8 exception_code)
{
    LogIrqReceived(stack_frame, exception_code);
}

// ------------------------------
// IRQ8: real time clock
// ------------------------------

void isr_40(void *const stack_frame, const u8 exception_code)
{
    LogIrqReceived(stack_frame, exception_code);
}

// ------------------------------
// IRQ12: mouse
// ------------------------------

void isr_44(void *const stack_frame, const u8 exception_code)
{
    LogIrqReceived(stack_frame, exception_code);
}
}