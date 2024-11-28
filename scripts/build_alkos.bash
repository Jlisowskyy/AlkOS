#!/bin/bash

BUILD_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BUILD_SCRIPT_PATH="${BUILD_SCRIPT_DIR}/$(basename "$0")"

BUILD_DIR="${BUILD_SCRIPT_DIR}/../build"
SOURCE_DIR="${BUILD_SCRIPT_DIR}/.."
CMAKE_TOOLCHAIN_FILE_NAME="i686-elf-toolchain.cmake"
CMAKE_TOOLCHAIN_FILE_PATH="${SOURCE_DIR}/../cmake-scripts/${CMAKE_TOOLCHAIN_FILE_NAME}"

source "${BUILD_SCRIPT_DIR}/../env/pretty_print.bash"
source "${BUILD_SCRIPT_DIR}/../env/helpers.bash"

help() {
  echo "${BUILD_SCRIPT_PATH} [--run | -r] [--verbose | -v]"
  echo "Where:"
  echo "--run     | -r - flag to run the build process of AlkOS"
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

  pretty_info "Building AlkOS"

  mkdir -p "${BUILD_DIR}"

  base_runner "Failed to configure AlkOS" "${VERBOSE}" cmake -B "${BUILD_DIR}" -S "${SOURCE_DIR}" -DCMAKE_TOOLCHAIN_FILE="${CMAKE_TOOLCHAIN_FILE_PATH}"
  base_runner "Failed to build AlkOS" "${VERBOSE}" cmake --build "${BUILD_DIR}"

  pretty_success "AlkOS built successfully - Image is located in ${BUILD_DIR}/bin"
}

main "$@"
