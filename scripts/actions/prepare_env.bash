#!/bin/bash

PREPARE_ENV_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

"${PREPARE_ENV_SCRIPT_DIR}/../alkos_cli.bash" --install all
