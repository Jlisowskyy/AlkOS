          bits 32
          TAB equ 0x09
          NEWLINE equ 0x0A
; Error messages
          section .rodata
          ; Info messages
          global MESSAGE_INIT_ALKOS

          ; Error messages
          global MESSAGE_ERROR_NO_LONG_MODE
          global MESSAGE_ERROR_NO_MULTIBOOT
          global MESSAGE_ERROR_NO_CPUID
          global MESSAGE_ERROR_UNKNOWN

          ; Success messages
          global MESSAGE_CHECK_CPUID
          global MESSAGE_CHECK_LONG_MODE
          global MESSAGE_CHECK_MULTIBOOT

          global MESSAGE_ENABLE_LONG_MODE
          global MESSAGE_ENABLE_PAGING
          global MESSAGE_SETUP_PAGE_TABLES
          global MESSAGE_SETUP_PAGING

MESSAGE_INIT_ALKOS db "[Info]", TAB, "Initializing Alkos", NEWLINE, 0
MESSAGE_ERROR_NO_LONG_MODE db "[Error]", TAB, "No long mode support", NEWLINE, 0
MESSAGE_ERROR_NO_MULTIBOOT db "[Error]", TAB, "No multiboot support", NEWLINE, 0
MESSAGE_ERROR_NO_CPUID db "[Error]", TAB, "No CPUID support", NEWLINE, 0
MESSAGE_ERROR_UNKNOWN db "[Error]", TAB, "Unknown error", NEWLINE, 0

MESSAGE_CHECK_CPUID db "[OK]", TAB, "Detected CPUID support", NEWLINE, 0
MESSAGE_CHECK_LONG_MODE db "[OK]", TAB, "Detected long mode support", NEWLINE, 0
MESSAGE_CHECK_MULTIBOOT db "[OK]", TAB, "Detected Multiboot support", NEWLINE, 0

MESSAGE_ENABLE_LONG_MODE db "[OK]", TAB, "Enabled long mode", NEWLINE, 0
MESSAGE_ENABLE_PAGING db "[OK]", TAB, "Enabled paging", NEWLINE, 0
MESSAGE_SETUP_PAGE_TABLES db "[OK]", TAB, "Setup page tables", NEWLINE, 0
MESSAGE_SETUP_PAGING db "[OK]", TAB, "Setup paging", NEWLINE, 0
