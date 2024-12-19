          ; Error codes
          NO_ERROR equ 0
          ERROR_NO_MULTIBOOT equ 1
          ERROR_NO_CPUID equ 2
          ERROR_NO_LONG_MODE equ 3
          ERROR_NO_FRAMEBUFFER equ 4

          ; Messages
          SUCCESS_CHECK_CPUID equ 0xFFFE
          SUCCESS_CHECK_LONG_MODE equ 0xFFFD
          SUCCESS_CHECK_MULTIBOOT equ 0xFFFC

          SUCCESS_ENABLE_LONG_MODE equ 0xFFFB
          SUCCESS_ENABLE_PAGING equ 0xFFFA
          SUCCESS_SETUP_PAGE_TABLES equ 0xFFF9