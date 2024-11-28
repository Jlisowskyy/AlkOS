cmake_minimum_required(VERSION 3.13)
project(AlkOS LANGUAGES C CXX ASM_NASM)

# Enable languages
enable_language(C CXX ASM_NASM)

# Run script that sets up the i686-elf toolchain
# (In case of a custom toolchain installation,
# you can modify the script to point to the correct paths)
include(${CMAKE_SOURCE_DIR}/cmake-scripts/i686-elf-toolchain.cmake)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -O2 -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")

# Set linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ffreestanding -nostdlib")

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/linker.ld)

set(BOOT_ASM ${CMAKE_SOURCE_DIR}/src/boot.nasm)
set_source_files_properties(${BOOT_ASM} PROPERTIES LANGUAGE ASM_NASM)
set(KERNEL_SRC ${CMAKE_SOURCE_DIR}/src/stub/kernel.cpp)

# Add executable target
add_executable(alkos.bin ${BOOT_ASM} ${KERNEL_SRC})

# Set linker options
target_link_options(alkos.bin PRIVATE -T ${LINKER_SCRIPT} -nostdlib -lgcc)

# Ensure that NASM is used for assembly files
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
