#ifndef ARCH_X86_64_INCLUDE_ARCH_UTILS_HPP_
#define ARCH_X86_64_INCLUDE_ARCH_UTILS_HPP_

#include <defines.hpp>
#include <io.hpp>
#include <todo.hpp>
#include <types.hpp>

FAST_CALL void BlockHardwareInterrupts() { __asm__ volatile("cli"); }

FAST_CALL void EnableHardwareInterrupts() { __asm__ volatile("sti"); }

FAST_CALL void HaltCpu() { __asm__ volatile("hlt"); }

FAST_CALL void OsHang()
{
    while (true) {
        HaltCpu();
    }
}

FAST_CALL void OsHangNoInterrupts()
{
    BlockHardwareInterrupts();
    OsHang();
}

FAST_CALL void InvokeInterrupt(const u8 idx) { __asm__ volatile("int %0" : : "N"(idx)); }

FAST_CALL void QemuShutdown()
{
    outw(0x604, 0x2000);
    OsHang();
}

struct PACK CpuState final {
    enum GeneralPurposeRegisters {
        kRax,
        kRbx,
        kRcx,
        kRdx,
        kRsi,
        kRdi,
        kRbp,
        kRsp,
        kR8,
        kR9,
        kR10,
        kR11,
        kR12,
        kR13,
        kR14,
        kR15,
        kGprLast,
    };

    u64 general_purpose_registers[kGprLast];

    void DumpStateDescStr(char* buf, size_t size) { TODO_BY_THE_END_OF_MILESTONE0 }

    void DumpStateDesc() const;
};

[[nodiscard]] CpuState DumpCpuState();

#endif  // ARCH_X86_64_INCLUDE_ARCH_UTILS_HPP_
