/**
 * Stub kernel entry
 * */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__x86_64__)
#error "AlkOS needs to be compiled with a x86_64-elf compiler"
#endif

/* Make kernel entry C compatible */
extern "C"
{

    const char* kernel_main()
    {
        return "Hello, AlkOS!";
    }
}
