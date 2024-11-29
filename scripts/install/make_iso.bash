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
        if [ -z "$TARGET" ]; then
          TARGET="$1"
        elif [ -z "$SOURCE" ]; then
          SOURCE="$1"
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
  if [ -z "$TARGET" ] || [ -z "$SOURCE" ]; then
    dump_error "Both target and source must be provided!"
    exit 1
  fi

  if [ "$RUN" = false ] ; then
    dump_error "--run flag was not provided!"
    exit 1
  fi
}


main() {
  parse_args "$@"
  process_args

  if [ ! -d "$SOURCE" ]; then
    dump_error "Source directory does not exist: $SOURCE"
    exit 1
  fi

  if ! command -v grub-mkrescue &> /dev/null; then
    dump_error "grub-mkrescue is not installed!"
    exit 1
  fi

  pretty_info "Creating boot directory"
  base_runner "Failed to create boot directory" "${VERBOSE}" mkdir -p "${SOURCE}/boot"

  pretty_info "Creating grub.cfg file"
  base_runner "Failed to create grub.cfg" "${VERBOSE}" \
    echo "${MAKE_ISO_SCRIPT_GRUB_CONTENTS}" > "${SOURCE}/${MAKE_ISO_SCRIPT_GRUB_PATH_IN_ISO}"

  pretty_info "Creating .iso file $TARGET from $SOURCE"
  base_runner "Failed to create .iso file" "${VERBOSE}" \
    grub-mkrescue -o "${TARGET}" "${SOURCE}"
  pretty_info "Created .iso file $(basename "${TARGET}")"
}

main "$@"