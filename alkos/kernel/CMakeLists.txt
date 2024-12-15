project(kernel)

if (NOT DEFINED SYSROOT)
    message(FATAL_ERROR "SYSROOT is not defined")
endif ()

include_directories(include)
include_directories(${SYSROOT}/usr/include)
include_directories(test)

# Define ARCH_DIR
set(ARCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}")

# Additional test definitions
 add_compile_definitions(ALKOS_TEST=1)

file(GLOB_RECURSE ARCH_SOURCES
        "${ARCH_DIR}/*.cpp"
        "${ARCH_DIR}/*.c"
        "${ARCH_DIR}/*.s"
        "${ARCH_DIR}/*.S"
        "${ARCH_DIR}/*.asm"
        "${ARCH_DIR}/*.nasm"
)

set(ARCH_DIR_ASM_FLAGS "-I ${ARCH_DIR} -I ${ARCH_DIR}/utils -g -F dwarf")
foreach(ASM_FILE ${ARCH_SOURCES})
    get_source_file_property(CURRENT_COMPILE_FLAGS ${ASM_FILE} COMPILE_FLAGS)
    if (CURRENT_COMPILE_FLAGS)
        set(CURRENT_COMPILE_FLAGS "${CURRENT_COMPILE_FLAGS} ${ARCH_DIR_ASM_FLAGS}")
    else ()
        set(CURRENT_COMPILE_FLAGS "${ARCH_DIR_ASM_FLAGS}")
    endif ()
    set_source_files_properties(${ASM_FILE} PROPERTIES COMPILE_FLAGS ${CURRENT_COMPILE_FLAGS})
endforeach()

# Add kernel sources
file(GLOB KERNEL_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.s"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.S"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.nasm"
)

# Build the kernel executable
add_executable(alkos.kernel
        ${ARCH_SOURCES}
        ${BOOT_ASM}
        ${KERNEL_SOURCES}
        )

# Set linker flags
set(LINKER_SCRIPT "${ARCH_DIR}/linker.ld")
set_target_properties(alkos.kernel PROPERTIES
        LINK_FLAGS "-T ${LINKER_SCRIPT} -nostdlib -lgcc"
)

# Link against libk and gcc
target_link_libraries(alkos.kernel PRIVATE
        ${SYSROOT}/usr/lib/libk.a
        gcc
)

# Ensure sysroot/boot directory exists
file(MAKE_DIRECTORY ${SYSROOT}/boot)

# Set kernel output directory to sysroot/boot
set_target_properties(alkos.kernel PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${SYSROOT}/boot
        OUTPUT_NAME "alkos.kernel"
)
