target_include_directories(alkos.kernel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE ARCH_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.S"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.nasm"
)

list(FILTER ARCH_SOURCES EXCLUDE REGEX ".*/(crti\\.nasm|crtn\\.nasm)$")

# Apply flags to kernel target
target_compile_options(alkos.kernel PRIVATE ${ARCH_C_FLAGS} ${ARCH_CXX_FLAGS})
set_source_files_properties(${ARCH_SOURCES} PROPERTIES COMPILE_FLAGS "${ARCH_ASM_FLAGS}")

# Add kernel sources
target_sources(alkos.kernel PRIVATE ${ARCH_SOURCES})

# Set linker script and link flags
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
set_target_properties(alkos.kernel PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT} -nostdlib -lgcc")

################################################################################
#                             Convert 32 bit files                             #
################################################################################
file(GLOB_RECURSE ARCH_SOURCES_32
        "${CMAKE_CURRENT_SOURCE_DIR}/*_32.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*_32.c"
)

if(ARCH_SOURCES_32)
    add_library(alkos.kernel32 OBJECT
            ${ARCH_SOURCES_32}
    )
    target_include_directories(alkos.kernel32 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    set_target_properties(alkos.kernel32 PROPERTIES
            COMPILE_FLAGS "-m32"
    )

    target_compile_definitions(alkos.kernel32 PRIVATE
            __i386__=1
    )

    add_dependencies(alkos.kernel alkos.kernel32)


    set(ELF64_CONVERTER "${CMAKE_CURRENT_SOURCE_DIR}/convert_to_elf64.bash")
    set(POST_ARCH_ACTION ${ELF64_CONVERTER} ${CMAKE_OBJCOPY} \"$<TARGET_OBJECTS:alkos.kernel32>\" PARENT_SCOPE)

    # link 32bit objects to kernel
    target_link_libraries(alkos.kernel PRIVATE
            $<TARGET_OBJECTS:alkos.kernel32>
    )

endif()

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

message(STATUS "CRTI_OBJ: ${CRTI_OBJ}, CRTN_OBJ: ${CRTN_OBJ}")

add_dependencies(alkos.kernel alkos.kernel.crts)
