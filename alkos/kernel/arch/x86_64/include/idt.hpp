#ifndef ARCH_X86_64_INTERRUPTS_IDT_HPP_
#define ARCH_X86_64_INTERRUPTS_IDT_HPP_

#include <types.hpp>

// ------------------------------
// Crucial defines
// ------------------------------

static constexpr u16 kIrq1Offset = 0x20;
static constexpr u16 kIrq2Offset = 0x28;

// ------------------------------
// Data layout
// ------------------------------

struct PACK IsrStackFrame {
    u64 rip;
    u64 cs;
    u64 rflags;
    u64 rsp;
};

struct PACK IsrErrorStackFrame {
    u64 error_code;
    IsrStackFrame isr_stack_frame;
};

// ------------------------------
// Functions
// ------------------------------

void IdtInit();

#endif  // ARCH_X86_64_INTERRUPTS_IDT_HPP_
