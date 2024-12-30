/* internal includes */
#include <stdio.h>
#include <arch_utils.hpp>
#include <kernel_assert.hpp>
#include <terminal.hpp>

CpuState DumpCpuState()
{
    CpuState cpu_state{};

    __asm__ volatile("movq %%rax, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRax]));
    __asm__ volatile("movq %%rbx, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRbx]));
    __asm__ volatile("movq %%rcx, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRcx]));
    __asm__ volatile("movq %%rdx, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRdx]));
    __asm__ volatile("movq %%rsi, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRsi]));
    __asm__ volatile("movq %%rdi, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRdi]));
    __asm__ volatile("movq %%rbp, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRbp]));
    __asm__ volatile("movq %%rsp, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kRsp]));
    __asm__ volatile("movq %%r8, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR8]));
    __asm__ volatile("movq %%r9, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR9]));
    __asm__ volatile("movq %%r10, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR10]));
    __asm__ volatile("movq %%r11, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR11]));
    __asm__ volatile("movq %%r12, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR12]));
    __asm__ volatile("movq %%r13, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR13]));
    __asm__ volatile("movq %%r14, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR14]));
    __asm__ volatile("movq %%r15, %0" : "=m"(cpu_state.general_purpose_registers[CpuState::kR15]));

    return cpu_state;
}

void CpuState::DumpStateDesc() const
{
    static constexpr size_t kSizeBuff = 2048;

    static constexpr const char* kRegNames[]{"rax", "rbx", "rcx", "rdx", "rsi", "rdi",
                                             "rbp", "rsp", "r8",  "r9",  "r10", "r11",
                                             "r12", "r13", "r14", "r15"};

    char buff[kSizeBuff];

    size_t offset = 0;

    for (int i = 0; i < kGprLast && offset < kSizeBuff; ++i) {
        int written = snprintf(
            buff + offset, kSizeBuff - offset, "%s: 0x%016llx\n", kRegNames[i],
            general_purpose_registers[i]
        );

        offset += written;
        ASSERT_NEQ(offset, kSizeBuff);
    }

    TerminalWriteString(buff);
}
