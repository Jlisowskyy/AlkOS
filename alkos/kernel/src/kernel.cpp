#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__x86_64__)
#error "AlkOS needs to be compiled with a x86_64-elf compiler"
#endif

#ifdef ALKOS_TEST
#include <tester.hpp>
#endif

#include <init.hpp>

static void KernelRun() {
}

extern "C" const char *KernelMain() {
    KernelInit();

#ifdef ALKOS_TEST

    VERIFY_TEST_TYPE(ALKOS_TEST)
    RunTest(static_cast<TestType>(ALKOS_TEST));
    return "Hello from AlkOS test!";

#endif

    KernelRun();
    return "Hello from AlkOS!";
}
