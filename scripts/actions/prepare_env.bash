#!/bin/bash

PREPARE_ENV_SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

"${PREPARE_ENV_SCRIPT_DIR}/../alkos_cli.bash" --install all
