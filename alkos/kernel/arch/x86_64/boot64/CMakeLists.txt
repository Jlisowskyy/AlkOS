message(STATUS "Configuring boot64")
set(KERNEL_EXECUTABLES ${KERNEL_EXECUTABLES} alkos.kernel PARENT_SCOPE)
target_include_directories(alkos.kernel PRIVATE ${COMMON_INCLUDE_DIRS})

file(GLOB_RECURSE ARCH_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.S"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.nasm"
)
list(APPEND ARCH_SOURCES ${ARCH_COMMON_SOURCES})
list(FILTER ARCH_SOURCES EXCLUDE REGEX ".*/(crti\\.nasm|crtn\\.nasm)$")

# Apply flags to the kernel target
target_compile_options(alkos.kernel PRIVATE ${ARCH_C_FLAGS} ${ARCH_CXX_FLAGS})
set_source_files_properties(${ARCH_SOURCES} PROPERTIES COMPILE_FLAGS "${ARCH_ASM_FLAGS}")

# Add kernel sources
target_sources(alkos.kernel PRIVATE ${ARCH_SOURCES})

# Set linker script and flags
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
target_link_options(alkos.kernel PRIVATE
        -T ${LINKER_SCRIPT}     # Linker script
        -nostdlib               # No standard libs
        -z max-page-size=0x1000 # Maximum page size (For 1k alignment)
        -n                      # Omit default mem map
        -lgcc                   # Link against GCC
)

################################################################################
#                           CXX Global Constructors                            #
################################################################################

set(SRTI_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cxx/crti.nasm")
set(SRTN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cxx/crtn.nasm")

add_library(alkos.kernel.crts OBJECT ${SRTI_FILE} ${SRTN_FILE})

# Retrieve GCC provided CRT
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

# Override the link line to include CRT objects in the correct order, propagate to parent scope
set(CRTEND_OBJ "${CRTEND_OBJ}" PARENT_SCOPE)
set(CRTBEGIN_OBJ "${CRTBEGIN_OBJ}" PARENT_SCOPE)
set(CRTI_OBJ "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/alkos.kernel.crts.dir/cxx/crti.nasm.obj" PARENT_SCOPE)
set(CRTN_OBJ "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/alkos.kernel.crts.dir/cxx/crtn.nasm.obj" PARENT_SCOPE)


add_dependencies(alkos.kernel alkos.kernel.crts)
