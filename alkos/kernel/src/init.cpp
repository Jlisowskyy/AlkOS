#include <init.hpp>

/* internal includes */
#include <libssp.hpp>

void KernelInit() {
    __stack_chk_init();
}
