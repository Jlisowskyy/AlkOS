          bits 32

          %include "return_codes.nasm"

          section .text
          global check_cpuid
check_cpuid:
          ; Check if CPUID is supported by flipping the ID bit (bit 21) in
          ; the FLAGS register. If we can flip it, CPUID is avaliable.

          ; Copy FLAGS in to EAX via stack
          pushfd
          pop eax

          mov ecx, eax

          ; Flip the ID bit
          xor eax, 1 << 21

          ; Copy EAX to flags via stack
          push eax
          popfd

          ; Copy flags back to EAX (with the flipped bit if CPUID is supported)
          pushfd
          pop eax

          ; Restore FLAGS
          push ecx
          popfd

          ; If the bit was flipped, CPUID is supported
          xor eax, ecx
          jz .no_cpuid
          mov eax, SUCCESS_CHECK_CPUID
          ret
.no_cpuid:
          mov eax, ERROR_NO_CPUID
          ret
