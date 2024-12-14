#include "../include/init.hpp"

/* internal includes */
#include "../../libc/libssp/libssp.hpp"

void KernelInit() {
    __stack_chk_init();
}
