#include <panic.hpp>

void KernelPanic([[maybe_unused]] const char *msg) {
    int *ptr = reinterpret_cast<int *>(0xdeadc0de);
    *ptr = 1;
}
