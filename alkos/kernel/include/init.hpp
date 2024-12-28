#ifndef ALKOS_INCLUDE_INIT_HPP_
#define ALKOS_INCLUDE_INIT_HPP_

void KernelInit();

/**
 * @brief must be defined by arch code to set up all architecture dependent interrupt settings
 */
void KernelArchInit();

#endif // ALKOS_INCLUDE_INIT_HPP_
