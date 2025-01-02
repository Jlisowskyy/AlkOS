message(STATUS "Configuring boot32")
set(KERNEL_EXECUTABLES ${KERNEL_EXECUTABLES} alkos.boot32 PARENT_SCOPE)
set(BOOTABLE_KERNEL_EXECUTABLE alkos.boot32 PARENT_SCOPE)

file(GLOB_RECURSE ARCH_SOURCES_32
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.S"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.nasm"
)
list(APPEND ARCH_SOURCES_32 ${ARCH_COMMON_SOURCES})

file(GLOB_RECURSE ARCH_SOURCES_32_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_executable(alkos.boot32 ${ARCH_SOURCES_32})
# Include common headers
target_include_directories(alkos.boot32 PRIVATE ${COMMON_INCLUDE_DIRS})

# Apply flags to the kernel target
target_compile_options(alkos.boot32 PRIVATE ${ARCH_C_FLAGS} ${ARCH_CXX_FLAGS})
set_source_files_properties(${ARCH_SOURCES_32} PROPERTIES COMPILE_FLAGS "${ARCH_ASM_FLAGS}")
set_source_files_properties(${ARCH_SOURCES_32_CPP} PROPERTIES COMPILE_FLAGS "-m32")

# Set linker script and flags
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
target_link_options(alkos.boot32 PRIVATE
        -T ${LINKER_SCRIPT}     # Linker script
        -nostdlib               # No standard libs
        -z max-page-size=0x1000 # Maximum page size (For 1k alignment)
        -n                      # Omit default mem map
        -lgcc                   # Link against GCC
)
target_compile_definitions(alkos.boot32 PRIVATE
        __i386__=1
        __x86_64__=0
)

add_dependencies(alkos.kernel alkos.boot32)
