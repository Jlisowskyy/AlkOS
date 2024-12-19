#ifndef KERNEL_ARCH_X86_64_INCLUDE_CONSTANTS_HPP_
#define KERNEL_ARCH_X86_64_INCLUDE_CONSTANTS_HPP_

#ifdef __SERIAL_PORT_TEST__
static constexpr bool kSerialPortTest = true;
#else
static constexpr bool kSerialPortTest = false;
#endif // __SERIAL_PORT_TEST__

#define INLINE inline __attribute__((always_inline))

#endif // KERNEL_ARCH_X86_64_INCLUDE_CONSTANTS_HPP_
