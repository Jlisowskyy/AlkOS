#include <init.hpp>

/* internal includes */
#include <libssp.hpp>
#include <terminal.hpp>
#include <debug.hpp>

/* external init procedures */
extern "C" void enable_osxsave();

extern "C" void enable_sse();

extern "C" void enable_avx();

extern "C" void PreKernelInit() {
    TerminalInit();
    TRACE_INFO("Starting pre-kernel initialization...");

    /* NOTE: sequence is important */
    TRACE_INFO("Starting to setup CPU features...");
    TRACE_INFO("Setting up OS XSAVE...");
    enable_osxsave();
    TRACE_INFO("Setting up SSE...");
    enable_sse();
    TRACE_INFO("Setting up AVX...");
    enable_avx();
    TRACE_INFO("Finished cpu features setup.");

    TRACE_INFO("Setting up IDT...");
    InitInterrupts();
    TRACE_INFO("IDT setup done.");

    TRACE_INFO("Pre-kernel initialization finished.");
}

void KernelInit() {
    __stack_chk_init();
}
