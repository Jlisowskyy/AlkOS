          bits 32
; Error messages
          section .rodata
          global MESSAGE_ERROR_NO_LONG_MODE
          global MESSAGE_ERROR_NO_MULTIBOOT
          global MESSAGE_ERROR_NO_CPUID
          global MESSAGE_ERROR_UNKNOWN

MESSAGE_ERROR_NO_LONG_MODE db "Error: No long mode support", 0
MESSAGE_ERROR_NO_MULTIBOOT db "Error: No multiboot support", 0
MESSAGE_ERROR_NO_CPUID db "Error: No CPUID support", 0
MESSAGE_ERROR_UNKNOWN db "Error: Unknown error", 0
