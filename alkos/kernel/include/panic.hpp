#ifndef ALKOS_INCLUDE_PANIC_HPP_
#define ALKOS_INCLUDE_PANIC_HPP_

extern "C" __attribute__((__noreturn__)) void KernelPanic(const char *msg);

#endif // ALKOS_INCLUDE_PANIC_HPP_
