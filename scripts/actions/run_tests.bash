#!/bin/bash

INSTALL_TOOLCHAIN_ACTION_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

python "${INSTALL_TOOLCHAIN_ACTION_DIR}/../tests/runner.py"
