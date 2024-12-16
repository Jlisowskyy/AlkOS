#include <init.hpp>

/* internal includes */
#include <libssp.hpp>

extern "C" void PreKernelInit() {
    __stack_chk_init();
}

void KernelInit() {

}
