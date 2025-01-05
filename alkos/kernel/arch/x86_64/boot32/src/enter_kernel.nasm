          bits 32

          extern GDT64.Pointer
          extern GDT64.Data
          extern GDT64.Code

          section .text
          global enter_kernel
          ; void enter_kernel(void *kernel_entry, void *multiboot_info_addr)
          ;                   [ebp + 8]
enter_kernel:
          push ebp
          mov ebp, esp

          mov esi, [ebp + 8] ; Kernel entry point
          mov [k_ptr], esi

          lgdt [GDT64.Pointer]

          mov ax, GDT64.Data
          mov ss, ax
          mov ds, ax
          mov es, ax
          jmp GDT64.Code:.jmp_kernel
.jmp_kernel:
          mov edi, [ebp + 12] ; Multiboot info address
          mov eax, [k_ptr]
          dd 0
          jmp eax

          section .data
          align 16
k_ptr:    dd 0
