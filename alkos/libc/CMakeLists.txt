project(libc)

include_directories(include)
include_directories(${SYSROOT}/usr/include)

if (NOT DEFINED SYSROOT)
    message(FATAL_ERROR "SYSROOT is not defined")
endif ()

if (NOT DEFINED SYSTEM_LIB_TYPE)
    message(FATAL_ERROR "SYSTEM_LIB_TYPE is not defined")
endif ()

if (SYSTEM_LIB_TYPE STREQUAL "k" OR SYSTEM_LIB_TYPE STREQUAL "K")
    set(LIB_NAME "k")
elseif (SYSTEM_LIB_TYPE STREQUAL "c" OR SYSTEM_LIB_TYPE STREQUAL "C")
    set(LIB_NAME "c")
else ()
    message(FATAL_ERROR "Unknown SYSTEM_LIB_TYPE : ${SYSTEM_LIB_TYPE}")
endif ()

file(GLOB_RECURSE LIB_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Build the library
add_library(lib${LIB_NAME} STATIC ${LIB_SOURCES})
set_target_properties(lib${LIB_NAME} PROPERTIES OUTPUT_NAME "${LIB_NAME}")
target_compile_options(lib${LIB_NAME} PRIVATE ${LIB_SYSTEM_FLAGS})

# Ensure sysroot directories exist
file(MAKE_DIRECTORY ${SYSROOT}/usr/include)
file(MAKE_DIRECTORY ${SYSROOT}/usr/lib)

# Set library output directory to sysroot/usr/lib
set_target_properties(lib${LIB_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${SYSROOT}/usr/lib
        LIBRARY_OUTPUT_DIRECTORY ${SYSROOT}/usr/lib
)

# Copy headers to sysroot during build
add_custom_command(TARGET lib${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/include ${SYSROOT}/usr/include
)

