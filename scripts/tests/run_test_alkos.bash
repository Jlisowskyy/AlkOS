#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CONF_PATH="${SCRIPT_DIR}/../conf.bash"

source "${CONF_PATH}"

exec qemu-system-x86_64 ${CONF_QEMU_ARGS} -display none -cdrom "${CONF_ISO_PATH}"
