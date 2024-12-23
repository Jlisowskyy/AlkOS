
/* internal includes */
#include <bit.hpp>
#include <defines.hpp>
#include <init.hpp>
#include <panic.hpp>

/* crucial defines */
static constexpr u32 kIdtEntries  = 256;
static constexpr u8 kDefaultFlags = 0x8E;

/* gdt kernel code offset */
extern "C" u32 kKernelCodeOffset;

/* isr strub table initialized in nasm */
extern "C" u64 *IsrStubTable;

// ------------------------------
// Data layout
// ------------------------------

struct PACK IdtEntry
{
    u16 isr_low;   // The lower 16 bits of the ISR's address
    u16 kernel_cs; // The GDT segment selector that the CPU will load into CS before calling the ISR
    u8 ist;        // The IST in the TSS that the CPU will load into RSP; set to zero for now
    u8 attributes; // Type and attributes; see the IDT page
    u16 isr_mid;   // The higher 16 bits of the lower 32 bits of the ISR's address
    u32 isr_high;  // The higher 32 bits of the ISR's address
    u32 reserved;  // Set to zero
};

struct PACK Idtr
{
    u16 limit;
    u64 base;
};

// ------------------------------
// Global data
// ------------------------------

alignas(32) static bool g_idtGuards[kIdtEntries];
alignas(32) static IdtEntry g_idt[kIdtEntries];
alignas(32) static Idtr g_idtr;

// ------------------------------
// Isrs
// ------------------------------

extern "C" NO_RET void DefaultExceptionHandler()
{
    KernelPanic("Unknown Exception caught -> default handler invoked.");
}

// ------------------------------
// Functions
// ------------------------------

static void IdtSetDescriptor(const u8 idx, const u64 isr, const u8 flags)
{
    IdtEntry &entry = g_idt[idx];

    entry.isr_low    = reinterpret_cast<uint64_t>(isr) & kBitMask16;
    entry.kernel_cs  = kKernelCodeOffset;
    entry.ist        = 0;
    entry.attributes = flags;
    entry.isr_mid    = (reinterpret_cast<uint64_t>(isr) >> 16) & kBitMask16;
    entry.isr_high   = (reinterpret_cast<uint64_t>(isr) >> 32) & kBitMask32;
    entry.reserved   = 0;

    g_idtGuards[idx] = true;
}

static void IdtInit()
{
    g_idtr.base  = reinterpret_cast<uintptr_t>(g_idt);
    g_idtr.limit = static_cast<u16>(sizeof(IdtEntry)) * kIdtEntries - 1;

    for (u8 idx = 0; idx < 32; idx++)
    {
        IdtSetDescriptor(idx, IsrStubTable[idx], kDefaultFlags);
    }

    __asm__ volatile("lidt %0" : : "m"(g_idtr)); // load the new IDT
    __asm__ volatile("sti");                     // set the interrupt flag
}

void InitInterrupts() { IdtInit(); }