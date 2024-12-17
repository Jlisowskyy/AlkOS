#ifndef ARCH_X86_64_COMP_HPP_
#define ARCH_X86_64_COMP_HPP_

#ifdef __i386__

#define DEF_MULTITARGET_FUNC(type, name, signature) \
    extern "C" __attribute__((section(".text32")))  type name##_32 signature

#else

#define DEF_MULTITARGET_FUNC(type, name, signature) \
    extern "C" type name signature

#endif // __i386__

#endif // ARCH_X86_64_COMP_HPP_
