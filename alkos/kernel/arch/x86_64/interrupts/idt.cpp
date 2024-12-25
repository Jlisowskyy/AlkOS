/* internal includes */
#include <memory.h>
#include <bit.hpp>
#include <debug.hpp>
#include <defines.hpp>
#include <kernel_assert.hpp>
#include <panic.hpp>
#include <temp.hpp>

TODO_BY_THE_END_OF_MILESTONE0
TODO_WHEN_SNPRINTF_EXISTS
/**
 * @todo Add various debug traces when snprintf available
 */

/* crucial defines */
static constexpr u32 kStubTableSize = 64;
static constexpr u32 kIdtEntries    = 256;
static constexpr u8 kDefaultFlags   = 0x8E;

/* gdt kernel code offset */
extern "C" u32 kKernelCodeOffset;

/* isr stub table initialized in nasm */
extern "C" void *IsrStubTable[];

// ------------------------------
// Data layout
// ------------------------------

/**
 * @brief Data layout of x86_64 interrupt service routines, refer to intel manual for details
 */
struct PACK IdtEntry {
    u16 isr_low;    // The lower 16 bits of the ISR's address
    u16 kernel_cs;  // The GDT segment selector that the CPU will load into CS before calling the
                    // ISR
    u8 ist;         // The IST in the TSS that the CPU will load into RSP; set to zero for now
    u8 attributes;  // Type and attributes; see the IDT page
    u16 isr_mid;    // The higher 16 bits of the lower 32 bits of the ISR's address
    u32 isr_high;   // The higher 32 bits of the ISR's address
    u32 reserved;   // Set to zero
};

/**
 * @brief Structure describing Idt position in memory
 */
struct PACK Idtr {
    u16 limit;
    u64 base;
};

// ------------------------------
// Global data
// ------------------------------

/* vector holding bool to detect double assignment */
alignas(32) static bool g_idtGuards[kIdtEntries];

/* global strcture defining isr specifics for each interrupt signal */
alignas(32) static IdtEntry g_idt[kIdtEntries];

/* holds information about the idt position in memory */
static Idtr g_idtr;

// ------------------------------
// Isrs
// ------------------------------

extern "C" NO_RET void DefaultExceptionHandler(const u8 exception_code)
{
    temp_DisplayNum(exception_code, "Received exception code");
    KernelPanic("Unknown Exception caught -> default handler invoked.");
}

extern "C" void LogExceptionReceived(const u8 exception_code)
{
    temp_DisplayNum(exception_code, "Received exception code");
}

// ------------------------------
// Functions
// ------------------------------

static void IdtSetDescriptor(const u8 idx, const u64 isr, const u8 flags)
{
    ASSERT_EQ(false, g_idtGuards[idx]);

    IdtEntry &entry = g_idt[idx];

    entry.isr_low    = isr & kBitMask16;
    entry.kernel_cs  = kKernelCodeOffset;
    entry.ist        = 0;
    entry.attributes = flags;
    entry.isr_mid    = (isr >> 16) & kBitMask16;
    entry.isr_high   = (isr >> 32) & kBitMask32;
    entry.reserved   = 0;

    g_idtGuards[idx] = true;
}

void IdtInit()
{
    ASSERT(kKernelCodeOffset < UINT16_MAX && "Kernel code offset out of range");
    ASSERT_NEQ(0, kKernelCodeOffset);

    g_idtr.base  = reinterpret_cast<uintptr_t>(g_idt);
    g_idtr.limit = static_cast<u16>(sizeof(IdtEntry)) * kIdtEntries - 1;

    /* cleanup flag vector */
    memset(g_idtGuards, 0, sizeof(g_idtGuards));

    /* zero IDT */
    memset(g_idt, 0, sizeof(g_idt));

    for (u8 idx = 0; idx < kStubTableSize; ++idx) {
        IdtSetDescriptor(idx, reinterpret_cast<u64>(IsrStubTable[idx]), kDefaultFlags);
    }

    __asm__ volatile("lidt %0" : : "m"(g_idtr));  // load the new IDT
    __asm__ volatile("sti");                      // set the interrupt flag

    TRACE_SUCCESS("IDT initialized");
}
