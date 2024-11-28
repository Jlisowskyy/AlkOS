cmake_minimum_required(VERSION 3.13)
project(AlkOS LANGUAGES C CXX ASM_NASM)

# Enable the ASM_NASM language
enable_language(ASM_NASM)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti")
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")

# Set NASM object format
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/linker.ld)

set(BOOT_ASM ${CMAKE_SOURCE_DIR}/src/boot.nasm)
set_source_files_properties(${BOOT_ASM} PROPERTIES LANGUAGE ASM_NASM)

set(KERNEL_SRC ${CMAKE_SOURCE_DIR}/src/stub/kernel.cpp)

add_executable(alkos.bin ${BOOT_ASM} ${KERNEL_SRC})

target_link_options(alkos.bin PRIVATE -T ${LINKER_SCRIPT} -nostdlib -lgcc)
