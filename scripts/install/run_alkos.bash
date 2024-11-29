#!/bin/bash

RUN_ALKOS_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
RUN_ALKOS_SCRIPT_PATH="${RUN_ALKOS_SCRIPT_DIR}/$(basename "$0")"
RUN_ALKOS_SOURCE_DIR="${RUN_ALKOS_SCRIPT_DIR}/../.."

source "${RUN_ALKOS_SOURCE_DIR}/scripts/utils/helpers.bash"
source "${RUN_ALKOS_SOURCE_DIR}/scripts/utils/pretty_print.bash"

QEMU_COMMAND="qemu-system-i386"
QEMU_ARGS="-kernel ${RUN_ALKOS_SOURCE_DIR}/build/bin/alkos.bin"

help() {
  echo "${RUN_ALKOS_SCRIPT_PATH} [--run | -r] [--install | -i] [--verbose | -v]"
  echo "Where:"
  echo "--run     | -r - flag to run AlkOS in QEMU"
  echo "--verbose | -v - flag to enable verbose output"
}

parse_args() {
  RUN=false
  VERBOSE=false
  while [[ $# -gt 0 ]]; do
    case $1 in
      -h|--help)
        help
        exit 0
        ;;
      -r|--run)
        RUN=true
        shift
        ;;
      -v|--verbose)
        VERBOSE=true
        shift
        ;;
      *)
        echo "Unknown argument: $1"
        exit 1
        ;;
    esac
  done
}

process_args() {
  if [ "$RUN" = false ] ; then
    dump_error "--run flag was not provided!"
    exit 1
  fi
}

main() {
  parse_args "$@"
  process_args

  pretty_info "Running AlkOS in QEMU"
  # shellcheck disable=SC2086
  # QEMU_ARGS is intentionally unquoted
  base_runner "Failed to run AlkOS in QEMU" "${VERBOSE}" "${QEMU_COMMAND}" ${QEMU_ARGS}
}

main "$@"