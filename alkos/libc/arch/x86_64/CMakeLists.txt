############################### Preparing target ###############################

add_library(lib${LIB_NAME}_32 STATIC ${LIB_SOURCES})
set_target_properties(lib${LIB_NAME}_32 PROPERTIES OUTPUT_NAME "${LIB_NAME}")

target_compile_options(lib${LIB_NAME}_32 PRIVATE -m32)

############################### Adding includes ###############################

target_include_directories(lib${LIB_NAME}_32 PUBLIC
        include
        ${SYSROOT}/usr/include
)
