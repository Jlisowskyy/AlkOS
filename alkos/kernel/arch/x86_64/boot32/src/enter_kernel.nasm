          bits 32

          extern GDT64.Pointer
          extern GDT64.Data
          extern GDT64.Code

          section .text
          global enter_kernel
          ; void enter_kernel(void *kernel_entry, LoaderData *loader_data)
          ;                   [ebp + 8]           [ebp + 12]
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
.jmp_kernel:                  ; https://wiki.osdev.org/Creating_a_64-bit_kernel_using_a_separate_loader
          mov edi, [ebp + 12] ; LoaderData address
          mov eax, [k_ptr]    ; This is transformed to mov rax, [k_ptr] and uses the double word reserved below
          dd 0                ; Trick the processor, contains high address of k_ptr
          jmp eax             ; This part is plain bad, tricking the processor is not the best thing to do here

          section .data
          align 16
k_ptr:    dd 0
