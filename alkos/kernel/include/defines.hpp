#ifndef KERNEL_INCLUDE_DEFINES_HPP_
#define KERNEL_INCLUDE_DEFINES_HPP_

#define PACK __attribute__ ((__packed__))

#define NO_RET __attribute__((noreturn))

#define FORCE_INLINE inline __attribute__((always_inline))

#define WRAP_CALL FORCE_INLINE

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef __SERIAL_PORT_TEST__
static constexpr bool kSerialPortTest = true;
#else
static constexpr bool kSerialPortTest = false;
#endif // __SERIAL_PORT_TEST__

#endif // KERNEL_INCLUDE_DEFINES_HPP_
