          bits 32

          %include "return_codes.nasm"
          %include "puts.nasm"

          extern MESSAGE_ERROR_NO_LONG_MODE
          extern MESSAGE_ERROR_NO_CPUID
          extern MESSAGE_ERROR_UNKNOWN
          extern MESSAGE_ERROR_NO_MULTIBOOT

          extern MESSAGE_CHECK_CPUID
          extern MESSAGE_CHECK_LONG_MODE
          extern MESSAGE_CHECK_MULTIBOOT

          extern MESSAGE_ENABLE_LONG_MODE
          extern MESSAGE_ENABLE_PAGING
          extern MESSAGE_SETUP_PAGE_TABLES

          section .text32
          global handle_return_code
          ; void check_and_handle_errors(uint8_t error_code);
handle_return_code:
          ; Try error codes
          cmp eax, NO_ERROR
          je .no_error
          cmp eax, ERROR_NO_LONG_MODE
          je .error_no_long_mode
          cmp eax, ERROR_NO_CPUID
          je .error_no_cpuid
          cmp eax, ERROR_NO_MULTIBOOT
          je .error_no_multiboot

          ; Try messages
          cmp eax, SUCCESS_CHECK_CPUID
          je .cpu_id_check
          cmp eax, SUCCESS_CHECK_LONG_MODE
          je .long_mode_check
          cmp eax, SUCCESS_CHECK_MULTIBOOT
          je .multiboot_check
          cmp eax, SUCCESS_ENABLE_LONG_MODE
          je .long_mode_enable
          cmp eax, SUCCESS_ENABLE_PAGING
          je .paging_enable
          cmp eax, SUCCESS_SETUP_PAGE_TABLES
          je .page_tables_setup


          ; Unknown error
          jmp .unknown_error

.no_error:
          ret

.error_no_long_mode:
          puts_32 MESSAGE_ERROR_NO_LONG_MODE
          jmp .hang32

.error_no_cpuid:
          puts_32 MESSAGE_ERROR_NO_CPUID
          jmp .hang32

.error_no_multiboot:
          puts_32 MESSAGE_ERROR_NO_MULTIBOOT
          jmp .hang32

.unknown_error:
          ; Unknown error
          puts_32 MESSAGE_ERROR_UNKNOWN
          jmp .hang32

.cpu_id_check:
          puts_32 MESSAGE_CHECK_CPUID
          ret

.hang32:
          cli
          hlt
          jmp .hang32

.long_mode_check:
          puts_32 MESSAGE_CHECK_LONG_MODE
          ret

.multiboot_check:
          puts_32 MESSAGE_CHECK_MULTIBOOT
          ret

.long_mode_enable:
          puts_32 MESSAGE_ENABLE_LONG_MODE
          ret

.paging_enable:
          puts_32 MESSAGE_ENABLE_PAGING
          ret

.page_tables_setup:
          puts_32 MESSAGE_SETUP_PAGE_TABLES
          ret
