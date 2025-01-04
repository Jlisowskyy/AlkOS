//
// Created by wookie on 1/2/25.
//
#include <terminal.hpp>
#include <debug.hpp>
#include <arch_utils.hpp>

extern "C" int PreKernelInit() {
    TerminalInit();
    TerminalWriteString("Starting pre-kernel initialization...\n");
    return 1;
}
