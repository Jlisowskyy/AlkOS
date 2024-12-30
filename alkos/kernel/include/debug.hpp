#ifndef KERNEL_INCLUDE_DEBUG_HPP_
#define KERNEL_INCLUDE_DEBUG_HPP_

#include <defines.hpp>
#include <todo.hpp>
#include <debug_terminal.hpp>

// ------------------------------
// Traces
// ------------------------------

/**
 * @brief TRACE - simple macro for debugging purposes, works only when __USE_DEBUG_OUTPUT__ and
 * __DEBUG_ENABLE_TRACES__ are defined
 *
 * @note Currently supports using only compiled time strings
 *
 * @todo Add support for dynamic strings
 */

TODO_WHEN_SNPRINTF_EXISTS

#ifdef __DEBUG_ENABLE_TRACES__

#define TRACE(message) DebugTerminalWrite(message)

#else

#define TRACE(message)

#endif // __DEBUG_ENABLE_TRACES__

#define TRACE_FORMAT_LOCATION(message) __FILE__ " " TOSTRING(__LINE__) " " message "\n"
#define TRACE_FORMAT_ERROR(message)    "[ERROR] " TRACE_FORMAT_LOCATION(message)
#define TRACE_FORMAT_WARNING(message)  "[WARNING] " TRACE_FORMAT_LOCATION(message)
#define TRACE_FORMAT_INFO(message)     "[INFO] " TRACE_FORMAT_LOCATION(message)
#define TRACE_FORMAT_SUCCESS(message)  "[SUCCESS] " TRACE_FORMAT_LOCATION(message)

#define TRACE_ERROR(message)   TRACE(TRACE_FORMAT_ERROR(message))
#define TRACE_WARNING(message) TRACE(TRACE_FORMAT_WARNING(message))
#define TRACE_INFO(message)    TRACE(TRACE_FORMAT_INFO(message))
#define TRACE_SUCCESS(message) TRACE(TRACE_FORMAT_SUCCESS(message))

#endif // KERNEL_INCLUDE_DEBUG_HPP_
