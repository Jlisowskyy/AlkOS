#!/bin/bash

MAKE_ISO_ACTION_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

source "${MAKE_ISO_ACTION_DIR}/../utils/conf_handlers.bash"

source_conf_file
verify_conf_var_exists CONF_SYSROOT
verify_conf_var_exists CONF_ISO_PATH
verify_conf_var_exists CONF_KERNEL_MODULES
verify_conf_var_exists CONF_BOOTABLE_KERNEL_EXEC

"${MAKE_ISO_ACTION_DIR}/../install/make_iso.bash" "${CONF_ISO_PATH}" "${CONF_SYSROOT}" -e "${CONF_BOOTABLE_KERNEL_EXEC}" -m "${CONF_KERNEL_MODULES}" "$@"
