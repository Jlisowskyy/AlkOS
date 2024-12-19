#ifndef ARCH_X86_64_COMP_HPP_
#define ARCH_X86_64_COMP_HPP_

/**
 *  @file comp.h
 *  @brief File contains collection of macros necessary for i386/x86_64 multi-targeting compilation.
 *         Each function that is supposed to be compiled for both i386 and x86_64 should be declared
 *         With DEF_MULTITARGET_FUNC macro and every call to such function should be done using
 *         MULTITARGET_FUNC macro. There is single exception to this rule - functions that are
 *         declared static may be called directly without using MULTITARGET_FUNC macro (as are local to
 *         translation unit and are not visible outside).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __i386__

#define FUNC_SECTION __attribute__((section(".text32"))) __attribute__((optimize("align-functions=8")))

#define MULTITARGET_FUNC(name) name##_32

#else

#define FUNC_SECTION

#define MULTITARGET_FUNC(name) name

#endif // __i386__

#define DEF_MULTITARGET_FUNC(type, name, signature) \
    extern "C" FUNC_SECTION type MULTITARGET_FUNC(name) signature

#endif // ARCH_X86_64_COMP_HPP_
