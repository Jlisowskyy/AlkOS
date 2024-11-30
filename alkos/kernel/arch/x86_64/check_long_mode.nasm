          bits 32
          %include "error_codes.nasm"

          section .text32
          global check_long_mode
check_long_mode:
          ; Check the highest possible function supported by CPUID

          ; Source: Intel® 64 and IA-32 Architectures Software Developer’s Manual
          ; Volume 2A: CPUID-CPU Identification
          ; INPUT EAX = 0: Returns CPUID’s Highest Value for Basic Processor Information and the Vendor Identification String
          ; OUTPUT EAX = Highest value for basic processor information
          ; OUTPUT EBX, EDX, ECX = Vendor Identification String - "GenuineIntel" or "AuthenticAMD"
          mov eax, 0
          cpuid ; Highest possible function in EAX
          cmp eax, 0x80000000
          jl .no_long_mode

          ; Check if extended functions of CPUID are supported
          ; Source: Intel® 64 and IA-32 Architectures Software Developer’s Manual
          ; Volume 2A: CPUID-CPU Identification
          ; INPUT EAX = 80000000h: Get Highest Extended Function Supported
          ; OUTPUT EAX = Highest extended function supported
          mov eax, 0x80000000
          cpuid
          cmp eax, 0x80000001
          jl .no_long_mode

          ; Check if long mode is supported
          ; Source: Intel® 64 and IA-32 Architectures Software Developer’s Manual
          ; Volume 2A: CPUID-CPU Identification
          ; INPUT EAX = 80000001h: Extended Processor Info and Feature Bits
          ; Changes EAX, EBX, ECX, EDX
          ; We are interested in EDX bit 29
          mov eax, 0x80000001
          cpuid
          test edx, 1 << 29 ; Test if LM (Long Mode) bit is set
          jz .no_long_mode

          ; Long mode is supported
          mov al, NO_ERROR
          ret

.no_long_mode:
          mov al, ERROR_NO_LONG_MODE
          ret
