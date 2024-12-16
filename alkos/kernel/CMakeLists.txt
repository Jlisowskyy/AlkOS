project(kernel)

if (NOT DEFINED SYSROOT)
    message(FATAL_ERROR "SYSROOT is not defined")
endif ()

include_directories(include)
include_directories(${SYSROOT}/usr/include)
include_directories(test)

# Define ARCH_DIR
set(ARCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}")

# Additional test definitions
# add_compile_definitions(ALKOS_TEST=1)

file(GLOB_RECURSE ARCH_SOURCES
        "${ARCH_DIR}/*.cpp"
        "${ARCH_DIR}/*.c"
        "${ARCH_DIR}/*.s"
        "${ARCH_DIR}/*.S"
        "${ARCH_DIR}/*.asm"
        "${ARCH_DIR}/*.nasm"
)

# Remove crti.S and crtn.S from ARCH_SOURCES
list(FILTER ARCH_SOURCES EXCLUDE REGEX ".*/(crti\\.S|crtn\\.S)$")

set(ARCH_DIR_ASM_FLAGS "-I ${ARCH_DIR} -I ${ARCH_DIR}/utils -g -F dwarf")
foreach(ASM_FILE ${ARCH_SOURCES})
    get_source_file_property(CURRENT_COMPILE_FLAGS ${ASM_FILE} COMPILE_FLAGS)
    if (CURRENT_COMPILE_FLAGS)
        set(CURRENT_COMPILE_FLAGS "${CURRENT_COMPILE_FLAGS} ${ARCH_DIR_ASM_FLAGS}")
    else ()
        set(CURRENT_COMPILE_FLAGS "${ARCH_DIR_ASM_FLAGS}")
    endif ()
    set_source_files_properties(${ASM_FILE} PROPERTIES COMPILE_FLAGS ${CURRENT_COMPILE_FLAGS})
endforeach()

# Add kernel sources
file(GLOB KERNEL_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.s"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.S"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.nasm"
)
# Build the kernel executable
add_executable(alkos.kernel
        ${ARCH_SOURCES}
        ${BOOT_ASM}
        ${KERNEL_SOURCES}
)

# Set linker flags
set(LINKER_SCRIPT "${ARCH_DIR}/linker.ld")
set_target_properties(alkos.kernel PROPERTIES
        LINK_FLAGS "-T ${LINKER_SCRIPT} -nostdlib -lgcc"
)

# Link against libk and gcc
target_link_libraries(alkos.kernel PRIVATE
        ${SYSROOT}/usr/lib/libk.a
        gcc
)

# Ensure sysroot/boot directory exists
file(MAKE_DIRECTORY ${SYSROOT}/boot)

# Set kernel output directory to sysroot/boot
set_target_properties(alkos.kernel PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${SYSROOT}/boot
        OUTPUT_NAME "alkos.kernel"
)

# ------------------------------
# CXX global constructors
# ------------------------------

execute_process(
        COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -print-file-name=crtbegin.o
        OUTPUT_VARIABLE CRTBEGIN_OBJ
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -print-file-name=crtend.o
        OUTPUT_VARIABLE CRTEND_OBJ
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CXX_CONSTRUCTORS_SOURCES
        "${ARCH_DIR}/cxx/crti.S"
        "${ARCH_DIR}/cxx/crtn.S"
        "${CRTBEGIN_OBJ}"
        "${CRTEND_OBJ}"
)

add_library(alkos.kernel.crts OBJECT
        ${CXX_CONSTRUCTORS_SOURCES}
)

set_source_files_properties(
        ${CXX_CONSTRUCTORS_SOURCES}
        PROPERTIES LANGUAGE ASM
)

add_dependencies(alkos.kernel alkos.kernel.crts)

set(PREFIX x86_64-elf)
set(CMAKE_LINKER "${CROSS_COMPILER_PATH}/${PREFIX}-ld")

message(STATUS "Using linker: ${CMAKE_LINKER}")

# Retrieve object files to CRTI_OBJ and CRTN_OBJ
set(ALKOS_KERNEL_CRTS_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CRTI_OBJ "${ALKOS_KERNEL_CRTS_DIR}/CMakeFiles/alkos.kernel.crts.dir/arch/${ARCH}/cxx/crti.S.obj")
set(CRTN_OBJ "${ALKOS_KERNEL_CRTS_DIR}/CMakeFiles/alkos.kernel.crts.dir/arch/${ARCH}/cxx/crtn.S.obj")
message(STATUS "CRTI_OBJ: ${CRTI_OBJ}, CRTN_OBJ: ${CRTN_OBJ}")

set(CMAKE_CXX_LINK_EXECUTABLE
    "${CMAKE_CXX_COMPILER} <CMAKE_C_LINK_FLAGS> <FLAGS> <LINK_FLAGS>\
    ${CRTI_OBJ} ${CRTBEGIN_OBJ} \
    <OBJECTS> \
    ${CRTEND_OBJ} ${CRTN_OBJ} \
    -o <TARGET> <LINK_LIBRARIES>"
)
