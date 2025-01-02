message(STATUS "Configuring for x86_64")
# Note: most of the includes must be added to both alkos.kernel and alkos.kernel32

set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/common")

# Glob common files
file(GLOB_RECURSE ARCH_COMMON_SOURCES
        "${COMMON_DIR}/*.cpp"
        "${COMMON_DIR}/*.c"
        "${COMMON_DIR}/*.s"
        "${COMMON_DIR}/*.S"
        "${COMMON_DIR}/*.asm"
        "${COMMON_DIR}/*.nasm"
)

# Glob directories in common
file(GLOB COMMON_INCLUDE_DIRS
        "${COMMON_DIR}/include"
        "${COMMON_DIR}/drivers"
)

set(KERNEL_EXECUTABLES "")

# Boot32
add_subdirectory(boot32)

# Boot64
add_subdirectory(boot64)

set(CRTEND_OBJ "${CRTEND_OBJ}" PARENT_SCOPE)
set(CRTBEGIN_OBJ "${CRTBEGIN_OBJ}" PARENT_SCOPE)
set(CRTI_OBJ "${CRTI_OBJ}" PARENT_SCOPE)
set(CRTN_OBJ "${CRTN_OBJ}" PARENT_SCOPE)

set(KERNEL_EXECUTABLES ${KERNEL_EXECUTABLES} PARENT_SCOPE)
set(BOOTABLE_KERNEL_EXECUTABLE ${BOOTABLE_KERNEL_EXECUTABLE} PARENT_SCOPE)

if (NOT KERNEL_EXECUTABLES)
    message(FATAL_ERROR "No executable targets added (boot32, boot64)")
endif ()

message(STATUS "CRTI_OBJ: ${CRTI_OBJ}, CRTN_OBJ: ${CRTN_OBJ}")
