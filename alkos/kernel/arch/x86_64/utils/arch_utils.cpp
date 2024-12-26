/* internal includes */
#include <arch_utils.hpp>
#include <temp.hpp>

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
    TODO_BY_THE_END_OF_MILESTONE0

    TerminalWriteString("Dumping current cpu state:\n");
    temp_DisplayHex(general_purpose_registers[kRax], "rax: ");
    temp_DisplayHex(general_purpose_registers[kRbx], "rbx: ");
    temp_DisplayHex(general_purpose_registers[kRcx], "rcx: ");
    temp_DisplayHex(general_purpose_registers[kRdx], "rdx: ");
    temp_DisplayHex(general_purpose_registers[kRsi], "rsi: ");
    temp_DisplayHex(general_purpose_registers[kRdi], "rdi: ");
    temp_DisplayHex(general_purpose_registers[kRbp], "rbp: ");
    temp_DisplayHex(general_purpose_registers[kRsp], "rsp: ");
    temp_DisplayHex(general_purpose_registers[kR8],  "r8:  ");
    temp_DisplayHex(general_purpose_registers[kR9],  "r9:  ");
    temp_DisplayHex(general_purpose_registers[kR10], "r10: ");
    temp_DisplayHex(general_purpose_registers[kR11], "r11: ");
    temp_DisplayHex(general_purpose_registers[kR12], "r12: ");
    temp_DisplayHex(general_purpose_registers[kR13], "r13: ");
    temp_DisplayHex(general_purpose_registers[kR14], "r14: ");
    temp_DisplayHex(general_purpose_registers[kR15], "r15: ");
}

