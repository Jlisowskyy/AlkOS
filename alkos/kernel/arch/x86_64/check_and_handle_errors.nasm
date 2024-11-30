          bits 32
          %include "error_codes.nasm"
          extern MESSAGE_ERROR_NO_LONG_MODE
          extern MESSAGE_ERROR_NO_CPUID
          extern MESSAGE_ERROR_UNKNOWN
          extern MESSAGE_ERROR_NO_MULTIBOOT

          extern vga_print
          section .text32
          global check_and_handle_errors
          ; void check_and_handle_errors(uint8_t error_code);
          ; Function compares error_code with predefined error codes and handles them
          ; (By printing their respective error messages and hanging the system)
          ; Inputs:
          ;   AL = error_code
          ; Note:
          ;   In case of no error, it immediately returns
check_and_handle_errors:
          cmp al, NO_ERROR
          je .no_error
          cmp al, ERROR_NO_LONG_MODE
          je .error_no_long_mode
          cmp al, ERROR_NO_CPUID
          je .error_no_cpuid
          cmp al, ERROR_NO_MULTIBOOT
          je .error_no_multiboot
          ; Unknown error
          jmp .unknown_error

.no_error:
          ret

.error_no_long_mode:
          push MESSAGE_ERROR_NO_LONG_MODE
          call vga_print
          jmp .hang32

.error_no_cpuid:
          push MESSAGE_ERROR_NO_CPUID
          call vga_print
          jmp .hang32

.error_no_multiboot:
          push MESSAGE_ERROR_NO_MULTIBOOT
          call vga_print
          jmp .hang32

.unknown_error:
          ; Unknown error
          push MESSAGE_ERROR_UNKNOWN
          call vga_print
          jmp .hang32

.hang32:
          cli
          hlt
          jmp .hang32