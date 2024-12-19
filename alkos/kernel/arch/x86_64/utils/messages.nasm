          bits 32

          TAB equ 0x09
          NEWLINE equ 0x0A
          WHITESPACE equ 0x20

          ; Error messages
          section .rodata
          ; Info messages
          global MESSAGE_INIT_ALKOS
          global MESSAGE_INFO_JUMPING_TO_64
          global MESSAGE_INFO_IN_64

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

MESSAGE_INIT_ALKOS db "[INFO]", TAB, WHITESPACE, WHITESPACE, "Initializing Alkos", NEWLINE, 0
MESSAGE_INFO_JUMPING_TO_64 db "[INFO]", TAB, WHITESPACE, WHITESPACE, "Jumping to 64-bit mode", NEWLINE, 0
MESSAGE_INFO_IN_64 db "[OK]", TAB, WHITESPACE, WHITESPACE, "In 64-bit mode", NEWLINE, 0

MESSAGE_ERROR_NO_LONG_MODE db "[Error]", TAB, WHITESPACE, WHITESPACE, "No long mode support", NEWLINE, 0
MESSAGE_ERROR_NO_MULTIBOOT db "[Error]", TAB, WHITESPACE, WHITESPACE, "No multiboot support", NEWLINE, 0
MESSAGE_ERROR_NO_CPUID db "[Error]", TAB, WHITESPACE, WHITESPACE, "No CPUID support", NEWLINE, 0
MESSAGE_ERROR_UNKNOWN db "[Error]", TAB, WHITESPACE, WHITESPACE, "Unknown error", NEWLINE, 0

MESSAGE_CHECK_CPUID db "[OK]", TAB, WHITESPACE, WHITESPACE, "Detected CPUID support", NEWLINE, 0
MESSAGE_CHECK_LONG_MODE db "[OK]", TAB, WHITESPACE, WHITESPACE, "Detected long mode support", NEWLINE, 0
MESSAGE_CHECK_MULTIBOOT db "[OK]", TAB, WHITESPACE, WHITESPACE, "Detected Multiboot support", NEWLINE, 0

MESSAGE_ENABLE_LONG_MODE db "[OK]", TAB, WHITESPACE, WHITESPACE, "Enabled long mode", NEWLINE, 0
MESSAGE_ENABLE_PAGING db "[OK]", TAB, WHITESPACE, WHITESPACE, "Enabled paging", NEWLINE, 0
MESSAGE_SETUP_PAGE_TABLES db "[OK]", TAB, WHITESPACE, WHITESPACE, "Setup page tables", NEWLINE, 0
MESSAGE_SETUP_PAGING db "[OK]", TAB, WHITESPACE, WHITESPACE, "Setup paging", NEWLINE, 0