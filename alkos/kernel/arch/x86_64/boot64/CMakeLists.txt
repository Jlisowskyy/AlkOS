message(STATUS "Configuring boot64")

############################## Finding Sources ###############################

file(GLOB_RECURSE ARCH_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.S"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.nasm"
)

# Exclude specific files like crti.nasm and crtn.nasm
list(FILTER ARCH_SOURCES EXCLUDE REGEX ".*/(crti\\.nasm|crtn\\.nasm)$")

# If ENABLE_TESTS is NOT defined, exclude files from the 'tests/' directory
if (NOT DEFINED ENABLE_TESTS)
    list(FILTER ARCH_SOURCES EXCLUDE REGEX ".*/tests/.*")
endif ()

############################### Adding Sources ###############################

target_sources(alkos.kernel PRIVATE ${ARCH_SOURCES})

############################### Adding Headers ###############################

target_include_directories(alkos.kernel PRIVATE
        drivers
        include
)

################################ Linker Flags ################################

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
target_link_options(alkos.kernel PRIVATE
        -T ${LINKER_SCRIPT}     # Linker script
        -nostdlib               # No standard libs
        -z max-page-size=0x1000 # Maximum page size (For 1k alignment)
        -n                      # Omit default mem map
        -lgcc                   # Link against GCC
)

########################## CXX Global Constructors ###########################

set(SRTI_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cxx/crti.nasm")
set(SRTN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cxx/crtn.nasm")

add_library(alkos.kernel.crts OBJECT
        ${SRTI_FILE}
        ${SRTN_FILE}
)

set(CRTBEGIN_PATH_FILE "${CMAKE_CURRENT_BINARY_DIR}/crtbegin_path.txt")
set(CRTEND_PATH_FILE "${CMAKE_CURRENT_BINARY_DIR}/crtend_path.txt")

add_custom_command(
        OUTPUT ${CRTBEGIN_PATH_FILE}
        COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -print-file-name=crtbegin.o > ${CRTBEGIN_PATH_FILE}
        COMMENT "Getting crtbegin.o path"
        VERBATIM
)

add_custom_command(
        OUTPUT ${CRTEND_PATH_FILE}
        COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -print-file-name=crtend.o > ${CRTEND_PATH_FILE}
        COMMENT "Getting crtend.o path"
        VERBATIM
)

# Create custom target to ensure the commands run
add_custom_target(get_crt_paths
        DEPENDS ${CRTBEGIN_PATH_FILE} ${CRTEND_PATH_FILE}
)

# Make the kernel depend on getting CRT paths
add_dependencies(alkos.kernel get_crt_paths)

# Set up variables for the CRT objects
set(CRTBEGIN_OBJ "$<FILE:READ,${CRTBEGIN_PATH_FILE}>" PARENT_SCOPE)
set(CRTEND_OBJ "$<FILE:READ,${CRTEND_PATH_FILE}>" PARENT_SCOPE)
set(CRTI_OBJ "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/alkos.kernel.crts.dir/cxx/crti.nasm.o" PARENT_SCOPE)
set(CRTN_OBJ "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/alkos.kernel.crts.dir/cxx/crtn.nasm.o" PARENT_SCOPE)

add_dependencies(alkos.kernel alkos.kernel.crts)