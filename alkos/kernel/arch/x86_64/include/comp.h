#ifndef ARCH_X86_64_COMP_HPP_
#define ARCH_X86_64_COMP_HPP_

#ifdef __i386__

#define MULTITARGET_FUNC(name) name##_32

#define DEF_MULTITARGET_FUNC(type, name, signature) \
    extern "C" __attribute__((section(".text32")))  type MULTITARGET_FUNC(name) signature

#else

#define MULTITARGET_FUNC(name) name

#define DEF_MULTITARGET_FUNC(type, name, signature) \
    extern "C" type MULTITARGET_FUNC(name) signature

#endif // __i386__

#endif // ARCH_X86_64_COMP_HPP_
