cmake_minimum_required(VERSION 3.30)
project(AlkOS LANGUAGES C CXX ASM_NASM)

enable_language(ASM_NASM)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti")
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")

# Adjust flags according to the build type
if (BUILD_TYPE_UPPER STREQUAL "DEBUG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
elseif (BUILD_TYPE_UPPER STREQUAL "RELEASE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
else ()
    message(FATAL_ERROR "UNKNOWN BUILD TYPE: ${BUILD_TYPE_UPPER}")
endif ()

# Set Language standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

# Source files
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/linker.ld)
set(BOOT_ASM ${CMAKE_SOURCE_DIR}/src/boot.nasm)
set(KERNEL_SRC ${CMAKE_SOURCE_DIR}/src/stub/kernel.cpp)

# Build configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_source_files_properties(${BOOT_ASM} PROPERTIES LANGUAGE ASM_NASM)

add_executable(alkos.bin ${BOOT_ASM} ${KERNEL_SRC})
target_link_options(alkos.bin PRIVATE -T ${LINKER_SCRIPT} -nostdlib -lgcc)
