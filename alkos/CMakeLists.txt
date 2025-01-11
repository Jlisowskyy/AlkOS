cmake_minimum_required(VERSION 3.30)
project(AlkOS LANGUAGES C CXX ASM_NASM)
enable_language(ASM_NASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(PROJECT_NAME AlkOS)
set(PROJECT_VERSION "0.0")
set(PROJECT_AUTHOR "ALK Organisation")

################################################################################
#                                  Conf file                                   #
################################################################################

# Check if configuration was done
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/../scripts/conf.cmake")
    message(FATAL_ERROR "Cmake configuration (conf.cmake) file does not exists... Try running configure.bash script first!")
else ()
    message(STATUS "Detected configuration file. Updating variables...")
endif ()

include("${CMAKE_SOURCE_DIR}/../scripts/conf.cmake")

################################################################################
#                                  toolchain                                   #
################################################################################

include(${CMAKE_TOOLCHAIN_FILE})
include(${CMAKE_FLAGS_FILE})

################################################################################
#                                   Messages                                   #
################################################################################

message(STATUS "Building for architecture: ${ARCH}")

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Using build type: ${CMAKE_BUILD_TYPE}")

message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Using AR: ${CMAKE_AR}")
message(STATUS "Using Ranlib: ${CMAKE_RANLIB}")
message(STATUS "Using Linker: ${CMAKE_LINKER}")
message(STATUS "Using gdb: ${CMAKE_GDB}")

message(STATUS "Using C flags: ${CMAKE_C_FLAGS}")
message(STATUS "Using C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Using ASM_NASM flags: ${CMAKE_ASM_NASM_FLAGS}")

################################################################################
#                                   Sysroot                                    #
###############################################################################

set(SYSROOT ${CMAKE_BINARY_DIR}/sysroot)
set(CMAKE_SYSROOT ${SYSROOT})

# Create the sysroot directories
file(MAKE_DIRECTORY ${SYSROOT}/usr/include)
file(MAKE_DIRECTORY ${SYSROOT}/usr/lib)
file(MAKE_DIRECTORY ${SYSROOT}/boot)
file(MAKE_DIRECTORY ${SYSROOT}/boot/grub)

set(SYSTEM_LIB_TYPE k)

# Add the subdirectories
add_subdirectory(libc)
add_subdirectory(kernel)
if (NOT DEFINED BOOTABLE_KERNEL_EXECUTABLE)
    message(FATAL_ERROR "No primary kernel executable defined")
endif()

################################################################################
#                                Custom Targets                                #
################################################################################

set(MAKE_ISO_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/../scripts/install/make_iso.bash)
set(RUN_ALKOS_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/../scripts/install/run_alkos.bash)
set(ALKOS_ISO_PATH ${CMAKE_BINARY_DIR}/alkos.iso)
set(ALKOS_KERNEL_PATH ${CMAKE_BINARY_DIR}/sysroot/boot/alkos.kernel)

if (NOT EXISTS ${MAKE_ISO_SCRIPT_PATH})
    message(FATAL_ERROR "make_iso script not found: ${MAKE_ISO_SCRIPT_PATH}")
endif ()

if (NOT EXISTS ${RUN_ALKOS_SCRIPT_PATH})
    message(FATAL_ERROR "run_alkos script not found: ${RUN_ALKOS_SCRIPT_PATH}")
endif ()

message(STATUS "Using make_iso script: ${MAKE_ISO_SCRIPT_PATH}")
message(STATUS "Using run_alkos script: ${RUN_ALKOS_SCRIPT_PATH}")

# This is not a workaround, this is genuinely how you pass arguments to a bash script from CMake
string(REPLACE ";" " " KERNEL_MODULES_FORMATTED "${KERNEL_MODULES}")

add_custom_target(iso
        COMMAND ${MAKE_ISO_SCRIPT_PATH}
                ${ALKOS_ISO_PATH}
                ${SYSROOT}
                -v -r
                -e ${BOOTABLE_KERNEL_EXECUTABLE}
                -m "${KERNEL_MODULES_FORMATTED}"
        DEPENDS ${BOOTABLE_KERNEL_EXECUTABLE} ${KERNEL_MODULES} libk
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Custom target to run QEMU
add_custom_target(run
        COMMAND ${RUN_ALKOS_SCRIPT_PATH} ${ALKOS_ISO_PATH} -v -r
        DEPENDS iso
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# -s -S flags are used to start QEMU with GDB support, listening on port 1234 - tcp
add_custom_target(run-with-gdb
        COMMAND ${RUN_ALKOS_SCRIPT_PATH} ${ALKOS_ISO_PATH} -v -r -g
        DEPENDS iso
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(mount
        COMMAND sudo umount /mnt || true
        COMMAND sudo mount -o loop ${ALKOS_ISO_PATH} /mnt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
