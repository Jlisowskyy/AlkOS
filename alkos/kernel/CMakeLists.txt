project(kernecommonl LANGUAGES C CXX ASM_NASM)

if (NOT DEFINED SYSROOT)
    message(FATAL_ERROR "SYSROOT is not defined")
endif ()

if (NOT DEFINED ARCH)
    message(FATAL_ERROR "ARCH is not defined")
endif ()

# Arch has the option to define multiple kernel executables (like boot32, and main kernel)
# This is the boot executable that will be run by the bootloader
set(BOOTABLE_KERNEL_EXECUTABLE alkos.kernel)

# Base include directories
include_directories(include)
include_directories(${SYSROOT}/usr/include)
include_directories(test)
include_directories(abi)

add_compile_definitions(__ALKOS_KERNEL__=1)

# Uncomment to start test framework
add_compile_definitions(__ALKOS_TESTS_BUILD__=1)

# Defining this macro enables debug output
add_compile_definitions(__USE_DEBUG_OUTPUT__=1)

# Controls whether various traces should be printed to debug output
add_compile_definitions(__USE_DEBUG_TRACES__=1)

# Add kernel sources
# NOTE: kernel should not include any architecture specific code including assembly
file(GLOB KERNEL_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"

        # TODO: compile only when debug build
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
)

# Build the kernel executable
add_executable(alkos.kernel
        ${KERNEL_SOURCES}
)

# Include the architecture specific CMakeLists.txt file
add_subdirectory(arch/${ARCH})
if (NOT DEFINED KERNEL_EXECUTABLES)
    message(FATAL_ERROR "No kernel executables defined in arch/${ARCH}")
endif ()

if (NOT DEFINED BOOTABLE_KERNEL_EXECUTABLE)
    message(FATAL_ERROR "No primary kernel executable defined")
endif ()

# Link against libk and gcc
foreach (KERNEL_EXECUTABLE ${KERNEL_EXECUTABLES})
    target_link_libraries(${KERNEL_EXECUTABLE} PRIVATE ${SYSROOT}/usr/lib/libk.a gcc)
endforeach ()


# Ensure sysroot/boot directory exists
file(MAKE_DIRECTORY ${SYSROOT}/boot)

# Place each executable in the boot directory
foreach (KERNEL_EXECUTABLE ${KERNEL_EXECUTABLES})
    set_target_properties(${KERNEL_EXECUTABLE} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${SYSROOT}/boot
            OUTPUT_NAME ${KERNEL_EXECUTABLE}
    )
endforeach ()

# Note: architecture file have a possibility to define custom command to run before linking
if (DEFINED POST_ARCH_ACTION)
    message(STATUS "POST ACTION: ${POST_ARCH_ACTION}")

    add_custom_command(TARGET alkos.kernel PRE_LINK
            COMMAND ${POST_ARCH_ACTION}
            COMMENT "Running post build arch action"
    )
endif ()

# Set the linker to link objects in the correct order
# NOTE: ARCH cmake is expected to provide variables for global constructors according to the compiler needs
# NOTE: This linking must be done at this level as global constructors should be supported on each architecture
if (NOT DEFINED CRTI_OBJ OR NOT DEFINED CRTBEGIN_OBJ OR NOT DEFINED CRTEND_OBJ OR NOT DEFINED CRTN_OBJ)
    message(FATAL_ERROR "One of: crti.o, crtbegin.o, crtend.o, crtn.o is not defined. Those should be defined in the ARCH CMakeLists.txt file")
endif ()

set(CMAKE_CXX_LINK_EXECUTABLE
        "${CMAKE_CXX_COMPILER} <CMAKE_C_LINK_FLAGS> <FLAGS> <LINK_FLAGS>\
    ${CRTI_OBJ} ${CRTBEGIN_OBJ} <OBJECTS> ${CRTEND_OBJ} ${CRTN_OBJ} \
    -o <TARGET> <LINK_LIBRARIES>"
)

set(BOOTABLE_KERNEL_EXECUTABLE ${BOOTABLE_KERNEL_EXECUTABLE} PARENT_SCOPE) # Export the bootable kernel executable