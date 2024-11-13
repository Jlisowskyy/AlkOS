#!/bin/bash

HELPERS_SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

source "${HELPERS_SCRIPT_DIR}/pretty_print.bash"

dump_error() {
  help

  pretty_error "$1"
  exit 1
}

assert_no_argument() {
  if [ -z "$1" ]; then
    dump_error "ASSERT failed in ${BASH_SOURCE[1]} at line ${BASH_LINENO[0]}"
  fi
}


