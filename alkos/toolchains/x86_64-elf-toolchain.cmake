# Set the system name and processor
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# This makes CMake test the compiler by building a static library
# (So it doesn't need to link an executable against C standard library)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Define the path to your cross-compiler tools
set(CROSS_COMPILER_PATH "${CMAKE_CURRENT_LIST_DIR}/../../tools/bin")

# Specify the cross-compilers
set(PREFIX x86_64-elf)
message(STATUS "Building for target: ${PREFIX}")

set(CMAKE_C_COMPILER "${CROSS_COMPILER_PATH}/${PREFIX}-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILER_PATH}/${PREFIX}-g++")
set(CMAKE_AR "${CROSS_COMPILER_PATH}/${PREFIX}-ar")
set(CMAKE_RANLIB "${CROSS_COMPILER_PATH}/${PREFIX}-ranlib")
set(CMAKE_LINKER "${CROSS_COMPILER_PATH}/${PREFIX}-ld")
set(CMAKE_ASM_NASM_COMPILER "nasm")

# Inform the user about the selected tools
message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Using AR: ${CMAKE_AR}")
message(STATUS "Using Ranlib: ${CMAKE_RANLIB}")
message(STATUS "Using Linker: ${CMAKE_LINKER}")

# Setup the cmake environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # Don't search for programs on the host system
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)  # Only search for libraries in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)  # Only search for headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)  # Only search for packages in the target directories

# Optional: Specify NASM if it's not in your PATH
# set(CMAKE_ASM_NASM_COMPILER "/path/to/your/nasm")
