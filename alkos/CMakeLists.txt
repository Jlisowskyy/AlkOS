cmake_minimum_required(VERSION 3.30)
project(AlkOS LANGUAGES C CXX ASM_NASM)

enable_language(ASM_NASM)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti")
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf64")

# Adjust flags according to the build type
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -fno-inline -fstack-protector-all")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-inline -fstack-protector-all")
    set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -g -F dwarf")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
else ()
    message(FATAL_ERROR "UNKNOWN BUILD TYPE: ${CMAKE_BUILD_TYPE}")
endif ()

MESSAGE(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
MESSAGE(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "CMAKE_ASM_NASM_FLAGS: ${CMAKE_ASM_NASM_FLAGS}")

set(SYSROOT ${CMAKE_BINARY_DIR}/sysroot)
set(CMAKE_SYSROOT ${SYSROOT})
set(SYSTEM_LIB_TYPE k)
set(ARCH x86_64)

# Create the sysroot directories
file(MAKE_DIRECTORY ${SYSROOT}/usr/include)
file(MAKE_DIRECTORY ${SYSROOT}/usr/lib)
file(MAKE_DIRECTORY ${SYSROOT}/boot)
file(MAKE_DIRECTORY ${SYSROOT}/boot/grub)

# Add the subdirectories
add_subdirectory(libc)
add_subdirectory(kernel)

# Custom target to create ISO image
set(MAKE_ISO_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/../scripts/install/make_iso.bash)
set(RUN_ALKOS_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/../scripts/install/run_alkos.bash)
message(STATUS "Using make_iso script: ${MAKE_ISO_SCRIPT_PATH}")
message(STATUS "Using run_alkos script: ${RUN_ALKOS_SCRIPT_PATH}")
add_custom_target(iso
        COMMAND ${MAKE_ISO_SCRIPT_PATH} "${CMAKE_BINARY_DIR}/alkos.iso" ${SYSROOT} -v -r
        DEPENDS alkos.kernel
        DEPENDS libk
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Custom target to run QEMU
add_custom_target(run
        COMMAND ${RUN_ALKOS_SCRIPT_PATH} "${CMAKE_BINARY_DIR}/alkos.iso" -v -r
        DEPENDS iso
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# -s -S flags are used to start QEMU with GDB support, listening on port 1234 - tcp
add_custom_target(run-with-gdb
        COMMAND ${RUN_ALKOS_SCRIPT_PATH} -v -r -g
        DEPENDS iso
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(mount
        COMMAND sudo umount /mnt || true
        COMMAND sudo mount -o loop ${CMAKE_BINARY_DIR}/alkos.iso /mnt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
