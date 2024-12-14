

RUN_ALKOS_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
RUN_ALKOS_SCRIPT_PATH="${RUN_ALKOS_SCRIPT_DIR}/$(basename "$0")"
RUN_ALKOS_SCRIPT_SOURCE_DIR="${RUN_ALKOS_SCRIPT_DIR}/../.."

source "${RUN_ALKOS_SCRIPT_SOURCE_DIR}/scripts/utils/helpers.bash"
source "${RUN_ALKOS_SCRIPT_SOURCE_DIR}/scripts/utils/pretty_print.bash"

RUN_ALKOS_SCRIPT_QEMU_COMMAND="qemu-system-x86_64"
RUN_ALKOS_SCRIPT_QEMU_ARGS="-cdrom ${RUN_ALKOS_SCRIPT_SOURCE_DIR}/build/bin/alkos.iso"
RUN_ALKOS_SCRIPT_GDB_ARGS="-s -S"

help() {
  echo "${RUN_ALKOS_SCRIPT_PATH} [--run | -r] [--install | -i] [--verbose | -v]"
  echo "Where:"
  echo "--run     | -r - flag to run AlkOS in QEMU"
  echo "--verbose | -v - flag to enable verbose output"
  echo "--"
}

parse_args() {
  RUN_ALKOS_SCRIPT_RUN=false
  RUN_ALKOS_SCRIPT_VERBOSE=false
  RUN_ALKOS_SCRIPT_GDB=false
  while [[ $# -gt 0 ]]; do
    case $1 in
      -h|--help)
        help
        exit 0
        ;;
      -r|--run)
        RUN_ALKOS_SCRIPT_RUN=true
        shift
        ;;
      -v|--verbose)
        RUN_ALKOS_SCRIPT_VERBOSE=true
        shift
        ;;
      -g|--gdb)
        RUN_ALKOS_SCRIPT_GDB=true
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
  if [ "$RUN_ALKOS_SCRIPT_RUN" = false ] ; then
    dump_error "--run flag was not provided!"
    exit 1
  fi

  if [ "$RUN_ALKOS_SCRIPT_GDB" = true ] ; then
    RUN_ALKOS_SCRIPT_QEMU_ARGS="${RUN_ALKOS_SCRIPT_QEMU_ARGS} ${RUN_ALKOS_SCRIPT_GDB_ARGS}"
  fi
}

main() {
  parse_args "$@"
  process_args

  pretty_info "Running AlkOS in QEMU"
  # shellcheck disable=SC2086
  # RUN_ALKOS_SCRIPT_QEMU_ARGS is intentionally unquoted
  base_runner "Failed to run AlkOS in QEMU" "${RUN_ALKOS_SCRIPT_VERBOSE}" "${RUN_ALKOS_SCRIPT_QEMU_COMMAND}" ${RUN_ALKOS_SCRIPT_QEMU_ARGS}
}

main "$@"
