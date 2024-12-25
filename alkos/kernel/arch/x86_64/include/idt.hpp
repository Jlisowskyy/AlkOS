#ifndef ARCH_X86_64_INTERRUPTS_IDT_HPP_
#define ARCH_X86_64_INTERRUPTS_IDT_HPP_

#include <types.hpp>

static constexpr u16 kIrq1Offset = 0x20;
static constexpr u16 kIrq2Offset = 0x28;

void IdtInit();

#endif  // ARCH_X86_64_INTERRUPTS_IDT_HPP_
