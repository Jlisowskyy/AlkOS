#ifndef ALKOS_INCLUDE_PANIC_HPP_
#define ALKOS_INCLUDE_PANIC_HPP_

/**
 * @brief Stops the kernel from functioning and disables all necessary devices and processes.
 * @note This function should also dump relevant debug information to the terminal
 *       to help diagnose the issue.
 * @param msg A message providing additional information about the panic.
 */
extern "C" void KernelPanic(const char *msg);

#endif  // ALKOS_INCLUDE_PANIC_HPP_
