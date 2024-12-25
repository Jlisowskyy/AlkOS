#ifndef KERNEL_ARCH_X86_64_DRIVERS_PIC8259_PIC8259_HPP_
#define KERNEL_ARCH_X86_64_DRIVERS_PIC8259_PIC8259_HPP_

/* internal includes */
#include <defines.hpp>
#include <io.hpp>
#include <types.hpp>

// ------------------------------
// Crucial definitions
// ------------------------------

static constexpr u16 kPic1Addr    = 0x20;
static constexpr u16 kPic2Addr    = 0xA0;
static constexpr u16 kPic1Command = kPic1Addr;
static constexpr u16 kPic1Data   = kPic1Addr + 1;
static constexpr u16 kPic2Command = kPic2Addr;
static constexpr u16 kPic2Data   = kPic2Addr + 1;

// ------------------------------
// Functions
// ------------------------------

void InitPic8259(byte pic1_offset, byte pic2_offset);

/* Acknowledge PIC about end of interrupt */
FAST_CALL void Pic8259SendEOI(const u8 irq)
{
    /* code for End Of Interrupt */
    static constexpr byte kPicEOI = 0x20;

    if (irq >= 8)
    {
        outb(kPic2Command, kPicEOI);
    }

    outb(kPic1Command, kPicEOI);
}

#endif // KERNEL_ARCH_X86_64_DRIVERS_PIC8259_PIC8259_HPP_
