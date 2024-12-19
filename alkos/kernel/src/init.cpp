#include <init.hpp>

/* internal includes */
#include <libssp.hpp>
#include <terminal.hpp>

/* external init procedures */
extern "C" void enable_osxsave();

extern "C" void enable_sse();

extern "C" void enable_avx();

extern "C" void PreKernelInit() {
    /**
     * TODO:
     * 1. Enable interrupts
     * 2. GDT, IDT, TSS, etc.
     */

    TerminalInit();
    TerminalWriteString("\t  In 64-bit mode\n");
    __stack_chk_init();

    /* NOTE: sequence is important */
    enable_osxsave();
    enable_sse();
    enable_avx();
}

void KernelInit() {
}
