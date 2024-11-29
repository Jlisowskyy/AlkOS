cmake_minimum_required(VERSION 3.30)
project(AlkOS LANGUAGES C CXX ASM_NASM)

enable_language(ASM_NASM)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti")
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")

# Adjust flags according to the build type
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
else ()
    message(FATAL_ERROR "UNKNOWN BUILD TYPE: ${CMAKE_BUILD_TYPE}")
endif ()

set(SYSROOT ${CMAKE_BINARY_DIR}/sysroot)
set(CMAKE_SYSROOT ${SYSROOT})
set(SYSTEM_LIB_TYPE k)
set(ARCH i686)

# Create the sysroot directories
file(MAKE_DIRECTORY ${SYSROOT}/usr/include)
file(MAKE_DIRECTORY ${SYSROOT}/usr/lib)
file(MAKE_DIRECTORY ${SYSROOT}/boot)

# Add the subdirectories
add_subdirectory(libc)
add_subdirectory(kernel)

# Custom target to create ISO image
add_custom_target(iso
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/isodir/boot/grub
        COMMAND ${CMAKE_COMMAND} -E copy ${SYSROOT}/boot/alkos.kernel ${CMAKE_BINARY_DIR}/isodir/boot/alkos.kernel
        COMMAND ${CMAKE_COMMAND} -E echo "set timeout=0" > ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
        COMMAND ${CMAKE_COMMAND} -E echo "set default=0" >> ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
        COMMAND ${CMAKE_COMMAND} -E echo "menuentry \"AlkOS\" {" >> ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
        COMMAND ${CMAKE_COMMAND} -E echo "    multiboot /boot/alkos.kernel" >> ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
        COMMAND ${CMAKE_COMMAND} -E echo "}" >> ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
        COMMAND grub-mkrescue -o ${CMAKE_BINARY_DIR}/alkos.iso ${CMAKE_BINARY_DIR}/isodir
        DEPENDS alkos.kernel
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Custom target to run QEMU
add_custom_target(run
        COMMAND qemu-system-i386 -cdrom ${CMAKE_BINARY_DIR}/alkos.iso
        DEPENDS iso
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)