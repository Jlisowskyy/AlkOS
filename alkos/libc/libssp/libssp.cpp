// ------------------------------
// Includes
// ------------------------------

/* main include */
#include "libssp.hpp"

/* external includes */
#include <stdint.h>

/* internal includes */
#include "../include/defines.hpp"

#ifdef __ALKOS_KERNEL__
#include "../../kernel/include/panic.hpp"
#endif // __ALKOS_KERNEL__

#if __STDC_HOSTED__
#error "NOT IMPLEMENTED"
#endif // __STDC_HOSTED__

// ------------------------------
// Stack Check Variable
// ------------------------------

/* random init value, should be changed by init proc */
#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

volatile uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

// ------------------------------
// Stack Check Init
// ------------------------------

/**
 * Initialize the stack check guard variable for the hosted environment
 */
#if __STDC_HOSTED__

static void __stack_chk_init_hosted() {
}

#endif // __STDC_HOSTED__

/**
 * Initialize the stack check guard variable for the kernel environment
 */
#ifdef __ALKOS_KERNEL__

static void __stack_chk_init_kernel() {
    // TODO: Implement this
}

#endif // __ALKOS_KERNEL__

/**
 * Initialize the stack check guard variable
 */
void __stack_chk_init() {
#if __STDC_HOSTED__

    __stack_chk_init_hosted();

#else

    __stack_chk_init_kernel();

#endif
}

// ------------------------------
// Stack Check Fail
// ------------------------------

/**
 * Stack check fail for the kernel environment
 */
#ifdef __ALKOS_KERNEL__
API_CALL static void __stack_chk_fail_kernel() {
    KernelPanic("Stack smashing detected");
}

#endif // __ALKOS_KERNEL__

/**
 * Stack check fail for the hosted environment
 */
#if __STDC_HOSTED__

static void __stack_chk_fail_hosted() {
}

#endif // __STDC_HOSTED__

/**
 * Stack check fail
 */
extern "C" __attribute__((noreturn)) void __stack_chk_fail() {
#if __STDC_HOSTED__

    __stack_chk_fail_hosted();

#else

    __stack_chk_fail_kernel();

#endif
}
