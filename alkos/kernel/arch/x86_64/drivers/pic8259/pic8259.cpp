
/* internal includes */
#include <pic8259/pic8259.hpp>
#include <types.hpp>
#include <debug.hpp>

// ------------------------------
// Crucial defines
// ------------------------------

/* ICW - Initialization Command Word */

/**
 * In our case we have 4 stage configuration:
 * - stage 1 (ICW1) - starts initialization procedure and configures its specifics
 * - stage 2 (ICW2) - specifies indexes in IDT
 * - stage 3 (ICW3) - configures relationship between PICs
 * - stage 4 (ICW4) - additional stage allowing to change CPU mode
 *
 * Note: there is a lot more flags to consider on each of the stages listed above
 */

/* signals that we will use Icw 4 */
static constexpr byte kIcw1Icw4 = 0x01;

/* initialization started signal */
static constexpr byte kIcw1Init = 0x10;

static constexpr byte kIcw3Irq2Slave = 0b100;

static constexpr byte kIcw3SlavePicCascadeIdent = 0b10;

/* 8086/88 (MCS-80/85) mode */
static constexpr byte kIcw4_8086 = 0x01;

// ------------------------------
// Implementations
// ------------------------------

void InitPic8259(const byte pic1_offset, const byte pic2_offset)
{
    /* save masks */
    const byte pic1Mask = inb(kPic1Data);
    const byte pic2Mask = inb(kPic2Data);

    /* ICW1 - Start initialization in cascade mode */
    outb(kPic1Command, kIcw1Init | kIcw1Icw4);
    IoWait();
    outb(kPic2Command, kIcw1Init | kIcw1Icw4);
    IoWait();

    /* ICW2 - Setup IDT offsets */
    outb(kPic1Data, pic1_offset);
    IoWait();
    outb(kPic2Data, pic2_offset);
    IoWait();

    /* ICW3 - acknowledge pics about communication structure */
    outb(kPic1Data, kIcw3Irq2Slave);
    IoWait();

    outb(kPic2Data, kIcw3SlavePicCascadeIdent);
    IoWait();

    /* ICW4 - setup 8086 mode instead of 8080 */
    outb(kPic1Data, kIcw4_8086);
    IoWait();

    outb(kPic2Data, kIcw4_8086);
    IoWait();

    /* restore saved masks */
    outb(kPic1Data, pic1Mask);
    outb(kPic2Data, pic2Mask);

    TRACE_SUCCESS("PIC units correctly initialized...");
}
