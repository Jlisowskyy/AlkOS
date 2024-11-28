# Set the system name and processor
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

# Define the path to your cross-compiler tools
set(CROSS_COMPILER_PATH "${CMAKE_SOURCE_DIR}/tools/bin")

# Specify the cross-compilers
set(CMAKE_C_COMPILER "${CROSS_COMPILER_PATH}/i686-elf-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILER_PATH}/i686-elf-g++")
set(CMAKE_AR "${CROSS_COMPILER_PATH}/i686-elf-ar")
set(CMAKE_RANLIB "${CROSS_COMPILER_PATH}/i686-elf-ranlib")

# Optional: Specify NASM if it's not in your PATH
# set(CMAKE_ASM_NASM_COMPILER "/path/to/your/nasm")

# This makes CMake test the compiler by building a static library
# (So it doesn't need to link an executable against C standard library)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
