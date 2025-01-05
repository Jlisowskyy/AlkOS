message(STATUS "Configuring for x86_64")

############################# Arch Dependent Vars ############################
set(KERNEL_MODULES "")
set(BOOTABLE_KERNEL_EXECUTABLE "")

set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/common")

################################ Common Files ################################
file(GLOB_RECURSE ARCH_COMMON_SOURCES
        "${COMMON_DIR}/*.cpp"
        "${COMMON_DIR}/*.c"
        "${COMMON_DIR}/*.s"
        "${COMMON_DIR}/*.S"
        "${COMMON_DIR}/*.asm"
        "${COMMON_DIR}/*.nasm"
)

file(GLOB COMMON_INCLUDE_DIRS
        "${COMMON_DIR}/include"
        "${COMMON_DIR}/drivers"
        "${COMMON_DIR}/abi"
)

################################### Boot32 ###################################
include(ExternalProject)

# Encode the lists to pass them to the boot32 project # TODO: This is a hack, fix it
string(REPLACE ";" "|" COMMON_INCLUDE_DIRS_ENCODED "${COMMON_INCLUDE_DIRS}")
string(REPLACE ";" "|" ARCH_COMMON_SOURCES_ENCODED "${ARCH_COMMON_SOURCES}")
string(REPLACE ";" "|" KERNEL_INCLUDE_DIRS_ENCODED "${KERNEL_INCLUDE_DIRS}")

ExternalProject_Add(
        boot32_external
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boot32
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/toolchains/i386-toolchain.cmake
        -DCMAKE_FLAGS_FILE=${CMAKE_SOURCE_DIR}/toolchains/i386-flags.cmake
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        -DCOMMON_DIR=${COMMON_DIR}
        -DCOMMON_INCLUDE_DIRS_ENCODED="${COMMON_INCLUDE_DIRS_ENCODED}"
        -DARCH_COMMON_SOURCES_ENCODED="${ARCH_COMMON_SOURCES_ENCODED}"
        -DKERNEL_INCLUDE_DIRS_ENCODED="${KERNEL_INCLUDE_DIRS_ENCODED}"
        BUILD_ALWAYS 1
)
add_executable(alkos.boot32 IMPORTED GLOBAL)
set_target_properties(alkos.boot32 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/alkos.boot32"
)
add_dependencies(alkos.boot32 boot32_external)

# Retrieve the bootable kernel executable # TODO: Rename this to alkos.loader
set(BOOTABLE_KERNEL_EXECUTABLE alkos.boot32)

################################### Boot64 ###################################
add_subdirectory(boot64)
set(KERNEL_MODULES ${KERNEL_MODULES} alkos.kernel)

########################## Back-Propagate Variables ##########################
set(CRTEND_OBJ "${CRTEND_OBJ}" PARENT_SCOPE)
set(CRTBEGIN_OBJ "${CRTBEGIN_OBJ}" PARENT_SCOPE)
set(CRTI_OBJ "${CRTI_OBJ}" PARENT_SCOPE)
set(CRTN_OBJ "${CRTN_OBJ}" PARENT_SCOPE)

set(BOOTABLE_KERNEL_EXECUTABLE ${BOOTABLE_KERNEL_EXECUTABLE} PARENT_SCOPE)
set(KERNEL_MODULES ${KERNEL_MODULES} PARENT_SCOPE)

############################### Error Checking ###############################
if (NOT BOOTABLE_KERNEL_EXECUTABLE)
    message(FATAL_ERROR "No primary kernel executable defined")
endif ()

if (NOT KERNEL_MODULES)
    message(STATUS "No kernel modules defined") # Kernel can be a mono-binary or a multi-binary
endif ()
message(STATUS "Bootable kernel executable: ${BOOTABLE_KERNEL_EXECUTABLE}")
message(STATUS "Kernel modules: ${KERNEL_MODULES}")

if (NOT CRTEND_OBJ)
    message(FATAL_ERROR "CRTEND_OBJ not defined")
endif ()

if (NOT CRTBEGIN_OBJ)
    message(FATAL_ERROR "CRTBEGIN_OBJ not defined")
endif ()

message(STATUS "CRTI_OBJ: ${CRTI_OBJ}, CRTN_OBJ: ${CRTN_OBJ}")
