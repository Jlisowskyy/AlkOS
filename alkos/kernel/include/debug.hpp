#ifndef KERNEL_INCLUDE_TRACE_HPP_
#define KERNEL_INCLUDE_TRACE_HPP_

/**
* @brief TRACE - simple macro for debugging purposes, works only when __SERIAL_PORT_TEST__ is defined
*
* @note Currently supports using only compiled time strings
*
* @todo Add support for dynamic strings
*/

#ifdef __SERIAL_PORT_TEST__

#include <serial_port_qemu/serial_qemu.hpp>

#define TRACE(message) QemuTerminalWriteString(message)

#else

#define TRACE(message)

#endif // __SERIAL_PORT_TEST__

#define TRACE_FORMAT_LOCATION(message) "__FILE__ __LINE__ __FUNCTION__ " message
#define TRACE_FORMAT_ERROR(message) "ERROR " TRACE_FORMAT_LOCATION(message)
#define TRACE_FORMAT_WARNING(message) "WARNING " TRACE_FORMAT_LOCATION(message)
#define TRACE_FORMAT_INFO(message) "INFO " TRACE_FORMAT_LOCATION(message)

#define TRACE_ERROR(message) TRACE(TRACE_FORMAT_ERROR(message))
#define TRACE_WARNING(message) TRACE(TRACE_FORMAT_WARNING(message))
#define TRACE_INFO(message) TRACE(TRACE_FORMAT_INFO(message))

#endif // KERNEL_INCLUDE_TRACE_HPP_
