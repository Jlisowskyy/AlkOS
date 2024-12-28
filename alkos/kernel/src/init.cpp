#include <init.hpp>

/* internal includes */
#include <libssp.hpp>
#include <terminal.hpp>
#include <debug.hpp>

extern "C" void PreKernelInit() {
    TerminalInit();
    TRACE_INFO("Starting pre-kernel initialization...");

    TRACE_INFO("Starting to setup CPU features...");
    KernelArchInit();
    TRACE_INFO("Finished cpu features setup.");

    TRACE_INFO("Pre-kernel initialization finished.");
}

void KernelInit() {
    __stack_chk_init();
}
