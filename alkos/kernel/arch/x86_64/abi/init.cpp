#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__x86_64__)
#error "AlkOS needs to be compiled with a x86_64-elf compiler"
#endif

/* internal includes */
#include <arch_utils.hpp>
#include <debug.hpp>
#include <idt.hpp>
#include <init.hpp>
#include <pic8259/pic8259.hpp>
#include <terminal.hpp>

/* external init procedures */
extern "C" void enable_osxsave();

extern "C" void enable_sse();

extern "C" void enable_avx();

extern "C" void PreKernelInit()
{
    TerminalInit();
    TRACE_INFO("Starting pre-kernel initialization...");

    TRACE_INFO("Starting to setup CPU features...");
    BlockHardwareInterrupts();

    /* NOTE: sequence is important */
    TRACE_INFO("Setting up OS XSAVE...");
    enable_osxsave();
    TRACE_INFO("Setting up SSE...");
    enable_sse();
    TRACE_INFO("Setting up AVX...");
    enable_avx();
    TRACE_INFO("Setting up PIC units...");
    InitPic8259(kIrq1Offset, kIrq2Offset);
    TRACE_INFO("Setting up IDT...");
    IdtInit();

    EnableHardwareInterrupts();
    TRACE_INFO("Finished cpu features setup.");

    TRACE_INFO("Pre-kernel initialization finished.");
}