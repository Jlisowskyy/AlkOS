# Set the system name and processor
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

# This makes CMake test the compiler by building a static library
# (So it doesn't need to link an executable against C standard library)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Define the path to your cross-compiler tools
set(CROSS_COMPILER_PATH "${CMAKE_CURRENT_LIST_DIR}/../tools/bin")

# Specify the cross-compilers
set(CMAKE_C_COMPILER "${CROSS_COMPILER_PATH}/i686-elf-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILER_PATH}/i686-elf-g++")
set(CMAKE_AR "${CROSS_COMPILER_PATH}/i686-elf-ar")
set(CMAKE_RANLIB "${CROSS_COMPILER_PATH}/i686-elf-ranlib")
set(CMAKE_LINKER "${CROSS_COMPILER_PATH}/i686-elf-ld")
set(CMAKE_ASM_NASM_COMPILER "nasm")

# Setup the cmake environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # Don't search for programs on the host system
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)  # Only search for libraries in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)  # Only search for headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)  # Only search for packages in the target directories

# Optional: Specify NASM if it's not in your PATH
# set(CMAKE_ASM_NASM_COMPILER "/path/to/your/nasm")

