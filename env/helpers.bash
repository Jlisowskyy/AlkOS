#!/bin/bash

HELPERS_SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
HELPERS_LOG_FILE="/tmp/alkOS_build.log"

source "${HELPERS_SCRIPT_DIR}/pretty_print.bash"

dump_error() {
  help

  pretty_error "$1"
  exit 1
}

assert_always() {
  dump_error "ASSERT failed in ${BASH_SOURCE[1]} at line ${BASH_LINENO[0]}"
}

assert_no_argument() {
  if [ -z "$1" ]; then
    dump_error "ASSERT NO ARGUMENT failed in ${BASH_SOURCE[1]} at line ${BASH_LINENO[0]}"
  fi
}

base_runner() {
    local dump_info="$1"
    shift
    local quiet="$1"
    shift

    if [ "$quiet" = true ]; then
        if ! "$@" > "$HELPERS_LOG_FILE" 2>&1; then
            dump_error "${dump_info}"
        fi
    else
        if ! "$@" 2>&1 | tee "$HELPERS_LOG_FILE"; then
            dump_error "${dump_info}"
        fi
    fi
}