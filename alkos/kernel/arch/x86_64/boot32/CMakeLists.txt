############################## Project Settings ##############################
cmake_minimum_required(VERSION 3.30)
project(boot32_project LANGUAGES C CXX ASM_NASM)
enable_language(ASM_NASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_definitions(__USE_DEBUG_OUTPUT__=1)
message(STATUS "Configuring boot32")

######################## Retrieving Passed Arguments #########################
set(CMAKE_FLAGS_FILE "${CMAKE_FLAGS_FILE}" CACHE FILEPATH "CMake flags file" FORCE)
set(COMMON_DIR "${COMMON_DIR}" CACHE PATH "Common directory" FORCE)
set(COMMON_INCLUDE_DIRS_ENCODED "${COMMON_INCLUDE_DIRS_ENCODED}" CACHE STRING "Encoded common include directories" FORCE)
set(ARCH_COMMON_SOURCES_ENCODED "${ARCH_COMMON_SOURCES_ENCODED}" CACHE STRING "Encoded common source files" FORCE)
# Convert the passed strings back to lists
string(REPLACE "|" ";" COMMON_INCLUDE_DIRS ${COMMON_INCLUDE_DIRS_ENCODED})
string(REPLACE "|" ";" ARCH_COMMON_SOURCES ${ARCH_COMMON_SOURCES_ENCODED})
string(REPLACE "|" ";" KERNEL_INCLUDE_DIRS ${KERNEL_INCLUDE_DIRS_ENCODED})
# Delete " from the strings
string(REPLACE "\"" "" COMMON_INCLUDE_DIRS "${COMMON_INCLUDE_DIRS}")
string(REPLACE "\"" "" ARCH_COMMON_SOURCES "${ARCH_COMMON_SOURCES}")
string(REPLACE "\"" "" KERNEL_INCLUDE_DIRS "${KERNEL_INCLUDE_DIRS}")
# TODO: THIS IS A HACK, FIX IT

message(STATUS "boot32 decoded COMMON_INCLUDE_DIRS_LIST: ${COMMON_INCLUDE_DIRS}")
message(STATUS "boot32 decoded ARCH_COMMON_SOURCES_LIST: ${ARCH_COMMON_SOURCES}")

############################## Finding Sources ###############################
file(GLOB_RECURSE ARCH_SOURCES_32
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.S"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.nasm"
)
list(APPEND ARCH_SOURCES_32 ${ARCH_COMMON_SOURCES})

file(GLOB_RECURSE ARCH_SOURCES_32_CPP
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

#################################### Exec ####################################
add_executable(alkos.boot32 ${ARCH_SOURCES_32})

############################## Finding Headers ###############################
target_include_directories(alkos.boot32 PRIVATE ${COMMON_INCLUDE_DIRS})
include_directories(include)
foreach (KERNEL_INCLUDE_DIR ${KERNEL_INCLUDE_DIRS})
    target_include_directories(alkos.boot32 PRIVATE ${KERNEL_INCLUDE_DIR})
endforeach ()

############################### Applying Flags ###############################
include(${CMAKE_FLAGS_FILE})

################################ Exec Flags ##################################
target_compile_options(alkos.boot32 PRIVATE ${ARCH_C_FLAGS} ${ARCH_CXX_FLAGS})
set_source_files_properties(${ARCH_SOURCES_32} PROPERTIES COMPILE_FLAGS "${ARCH_ASM_FLAGS}")

################################ Linker Flags ################################
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
target_link_options(alkos.boot32 PRIVATE
        -T ${LINKER_SCRIPT}     # Linker script
        -nostdlib               # No standard libs
        -z max-page-size=0x1000 # Maximum page size (For 1k alignment)
        -n                      # Omit default mem map
        -lgcc                   # Link against GCC
)
target_compile_definitions(alkos.boot32 PRIVATE
        __i386__=1
        __x86_64__=0
)
target_link_libraries(alkos.boot32 PRIVATE gcc)


################################## Install ###################################
install(TARGETS alkos.boot32 DESTINATION .) # Installs to SYSROOT (CMAKE_INSTALL_PREFIX)