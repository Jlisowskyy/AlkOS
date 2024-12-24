#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__x86_64__)
#error "AlkOS needs to be compiled with a x86_64-elf compiler"
#endif

/* internal includes */
#include <init.hpp>
#include <debug.hpp>

/* external init procedures */
extern "C" void enable_osxsave();

extern "C" void enable_sse();

extern "C" void enable_avx();

extern "C" void IdtInit();

void KernelArchInit()
{
    /* NOTE: sequence is important */
    TRACE_INFO("Setting up OS XSAVE...");
    enable_osxsave();
    TRACE_INFO("Setting up SSE...");
    enable_sse();
    TRACE_INFO("Setting up AVX...");
    enable_avx();
    TRACE_INFO("Setting up IDT...");
    IdtInit();
}
