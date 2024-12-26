#ifndef KERNEL_ARCH_X86_64_DRIVERS_PIC8259_PIC8259_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_PIC8259_PIC8259_HPP_

/* internal includes */
#include "pic8259.hpp"

#include <bit.hpp>
#include <defines.hpp>
#include <io.hpp>
#include <types.hpp>
#include <debug.hpp>

// ------------------------------
// Crucial definitions
// ------------------------------

static constexpr u16 kPic1Addr    = 0x20;
static constexpr u16 kPic2Addr    = 0xA0;
static constexpr u16 kPic1Command = kPic1Addr;
static constexpr u16 kPic1Data    = kPic1Addr + 1;
static constexpr u16 kPic2Command = kPic2Addr;
static constexpr u16 kPic2Data    = kPic2Addr + 1;

// ------------------------------
// Functions
// ------------------------------

void InitPic8259(byte pic1_offset, byte pic2_offset);

/* Acknowledge PIC about end of interrupt */
FAST_CALL void Pic8259SendEOI(const u8 irq)
{
    /* code for End Of Interrupt */
    static constexpr byte kPicEOI = 0x20;

    if (irq >= 8) {
        outb(kPic2Command, kPicEOI);
    }

    outb(kPic1Command, kPicEOI);

    TRACE_INFO("EOI sent to PIC...");
}

FAST_CALL void Pic8259SetIrqMaskValue(byte irq_line, const bool val)
{
    u16 port;

    if (irq_line < 8) {
        port = kPic1Data;
    } else {
        port = kPic2Data;
        irq_line -= 8;
    }

    byte mask = inb(port);
    SetBitValue(mask, irq_line, val);
    outb(port, mask);
}

FAST_CALL void Pic8259Disable()
{
    /* mask all interrupts possible */
    outb(kPic1Data, 0xff);
    outb(kPic2Data, 0xff);
}

#endif  // KERNEL_ARCH_X86_64_DRIVERS_PIC8259_PIC8259_HPP_
