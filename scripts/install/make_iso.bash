#!/bin/bash

MAKE_ISO_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
MAKE_ISO_SCRIPT_PATH="${MAKE_ISO_SCRIPT_DIR}/$(basename "$0")"

MAKE_ISO_SCRIPT_GRUB_CONTENTS="
set timeout=0
set default=0
menuentry \"AlkOS\" {
  multiboot /boot/alkos.kernel
  boot
}
"
MAKE_ISO_SCRIPT_GRUB_PATH_IN_ISO="boot/grub/grub.cfg"


source "${MAKE_ISO_SCRIPT_DIR}/../utils/helpers.bash"
source "${MAKE_ISO_SCRIPT_DIR}/../utils/pretty_print.bash"

help() {
  echo "${MAKE_ISO_SCRIPT_PATH} [target] [source] [--run | -r] [--verbose | -v]"
  echo "Creates a .iso for alkOS from the sysroot directory"
  echo "Where:"
  echo "target - path to the .iso file to create"
  echo "source - path to the sysroot directory of alkOS"
  echo "--run     | -r - flag to run make_iso.sh"
  echo "--verbose | -v - flag to enable verbose output"
}

parse_args() {
  MAKE_ISO_SCRIPT_RUN=false
  MAKE_ISO_SCRIPT_VERBOSE=false
  while [[ $# -gt 0 ]]; do
    case $1 in
      -h|--help)
        help
        exit 0
        ;;
      -r|--run)
        MAKE_ISO_SCRIPT_RUN=true
        shift
        ;;
      -v|--verbose)
        MAKE_ISO_SCRIPT_VERBOSE=true
        shift
        ;;
      *)
        if [ -z "$MAKE_ISO_SCRIPT_TARGET" ]; then
          MAKE_ISO_SCRIPT_TARGET="$1"
        elif [ -z "$MAKE_ISO_SCRIPT_SOURCE" ]; then
          MAKE_ISO_SCRIPT_SOURCE="$1"
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
  if [ -z "$MAKE_ISO_SCRIPT_TARGET" ] || [ -z "$MAKE_ISO_SCRIPT_SOURCE" ]; then
    dump_error "Both target and source must be provided!"
    exit 1
  fi

  if [ "$MAKE_ISO_SCRIPT_RUN" = false ] ; then
    dump_error "--run flag was not provided!"
    exit 1
  fi
}


main() {
  parse_args "$@"
  process_args

  if [ ! -d "$MAKE_ISO_SCRIPT_SOURCE" ]; then
    dump_error "Source directory does not exist: $MAKE_ISO_SCRIPT_SOURCE"
    exit 1
  fi

  if ! command -v grub-mkrescue &> /dev/null; then
    dump_error "grub-mkrescue is not installed!"
    exit 1
  fi

  pretty_info "Creating boot directory"
  base_runner "Failed to create boot directory" "${MAKE_ISO_SCRIPT_VERBOSE}" mkdir -p "${MAKE_ISO_SCRIPT_SOURCE}/boot"

  pretty_info "Creating grub.cfg file"
  base_runner "Failed to create grub.cfg" "${MAKE_ISO_SCRIPT_VERBOSE}" \
    echo "${MAKE_ISO_SCRIPT_GRUB_CONTENTS}" > "${MAKE_ISO_SCRIPT_SOURCE}/${MAKE_ISO_SCRIPT_GRUB_PATH_IN_ISO}"

  pretty_info "Creating .iso file $MAKE_ISO_SCRIPT_TARGET from $MAKE_ISO_SCRIPT_SOURCE"
  pretty_info "Ensuring that the path to the .iso file exists"
  base_runner "Failed to create path to .iso file" "${MAKE_ISO_SCRIPT_VERBOSE}" mkdir -p "$(dirname "${MAKE_ISO_SCRIPT_TARGET}")"
  base_runner "Failed to create .iso file" "${MAKE_ISO_SCRIPT_VERBOSE}" \
    grub-mkrescue -o "${MAKE_ISO_SCRIPT_TARGET}" "${MAKE_ISO_SCRIPT_SOURCE}"
  pretty_info "Created .iso file $(basename "${MAKE_ISO_SCRIPT_TARGET}")"
}

main "$@"
