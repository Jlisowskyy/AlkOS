message(STATUS "Configuring for x86_64")

############################# Arch Dependent Vars ############################

set(KERNEL_MODULES "")
set(BOOTABLE_KERNEL_EXECUTABLE "")

################################### Boot32 ###################################

add_subdirectory(boot32)
set_target_properties(alkos.loader PROPERTIES
        IMPORTED_LOCATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/alkos.loader"
)

# Retrieve the bootable kernel executable
set(BOOTABLE_KERNEL_EXECUTABLE alkos.loader)

################################### Boot64 ###################################

add_subdirectory(boot64)
set(KERNEL_MODULES ${KERNEL_MODULES} alkos.kernel)

################################### Common ###################################

add_subdirectory(common)

############################### Error Checking ###############################

if (NOT BOOTABLE_KERNEL_EXECUTABLE)
    message(FATAL_ERROR "No primary kernel executable defined")
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

########################## Back-Propagate Variables ##########################

set(CRTEND_OBJ "${CRTEND_OBJ}" PARENT_SCOPE)
set(CRTBEGIN_OBJ "${CRTBEGIN_OBJ}" PARENT_SCOPE)
set(CRTI_OBJ "${CRTI_OBJ}" PARENT_SCOPE)
set(CRTN_OBJ "${CRTN_OBJ}" PARENT_SCOPE)

set(BOOTABLE_KERNEL_EXECUTABLE ${BOOTABLE_KERNEL_EXECUTABLE} PARENT_SCOPE)
set(KERNEL_MODULES ${KERNEL_MODULES} PARENT_SCOPE)

########################## Link arch common ##########################

target_link_libraries(alkos.kernel PRIVATE arch.common)
target_link_libraries(alkos.loader PRIVATE arch.common.32)
