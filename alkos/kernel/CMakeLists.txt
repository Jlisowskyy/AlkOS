project(kernel LANGUAGES C CXX ASM_NASM)

if (NOT DEFINED SYSROOT)
    message(FATAL_ERROR "SYSROOT is not defined")
endif ()

if (NOT DEFINED ARCH)
    message(FATAL_ERROR "ARCH is not defined")
endif ()


# Base include directories
include_directories(include)
include_directories(${SYSROOT}/usr/include)
include_directories(test)

# Additional test definitions - refer to test/tester.hpp file for value specifics
# add_compile_definitions(ALKOS_TEST=1)

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
        ${KERNEL_SOURCES}
        )

# Include the architecture specific CMakeLists.txt file
add_subdirectory(arch/${ARCH})


# Link against libk and gcc
target_link_libraries(alkos.kernel PRIVATE
        ${SYSROOT}/usr/lib/libk.a
        gcc
)

# Ensure sysroot/boot directory exists
file(MAKE_DIRECTORY ${SYSROOT}/boot)

# Place the kernel in the sysroot/boot directory
set_target_properties(alkos.kernel PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${SYSROOT}/boot
        OUTPUT_NAME "alkos.kernel"
)
