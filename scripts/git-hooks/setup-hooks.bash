#!/bin/env bash
SETUP_HOOKS_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

set -e
git config core.hooksPath "${SETUP_HOOKS_SCRIPT_DIR}"
