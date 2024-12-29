

RUN_ALKOS_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
RUN_ALKOS_SCRIPT_PATH="${RUN_ALKOS_SCRIPT_DIR}/$(basename "$0")"
RUN_ALKOS_SCRIPT_SOURCE_DIR="${RUN_ALKOS_SCRIPT_DIR}/../.."

source "${RUN_ALKOS_SCRIPT_SOURCE_DIR}/scripts/utils/helpers.bash"
source "${RUN_ALKOS_SCRIPT_SOURCE_DIR}/scripts/utils/pretty_print.bash"

RUN_ALKOS_SCRIPT_QEMU_COMMAND="qemu-system-x86_64"
RUN_ALKOS_SCRIPT_GDB_ARGS="-s -S"
RUN_ALKOS_SCRIPT_QEMU_ARGS="-serial stdio -enable-kvm -cpu host -display default,show-cursor=on"
help() {
  echo "${RUN_ALKOS_SCRIPT_PATH} [alkos_iso_path] [--run | -r] [--verbose | -v]"
  echo "Where:"
  echo "alkos_iso_path - path to the .iso file"
  echo "--run     | -r - flag to run AlkOS in QEMU"
  echo "--verbose | -v - flag to enable verbose output"
  echo "--gdb     | -g - flag to run AlkOS in QEMU with GDB"
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
        if [ -z "$RUN_ALKOS_ISO_PATH" ]; then
          RUN_ALKOS_ISO_PATH="$1"
        else
          echo "Unknown argument: $1"
          exit 1
        fi
        shift
        ;;
    esac
  done
}

process_args() {
  if [ "$RUN_ALKOS_SCRIPT_RUN" = false ] ; then
    dump_error "--run flag was not provided!"
    exit 1
  fi

  if [ -z "$RUN_ALKOS_ISO_PATH" ]; then
    dump_error "No path to the .iso file was provided!"
    exit 1
  else
    RUN_ALKOS_SCRIPT_QEMU_ARGS="${RUN_ALKOS_SCRIPT_QEMU_ARGS} -cdrom ${RUN_ALKOS_ISO_PATH}"
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
