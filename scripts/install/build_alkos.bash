#!/bin/bash
BUILD_SCRIPT_CMAKE_TOOLCHAIN_FILE_NAME="x86_64-elf-toolchain.cmake"

BUILD_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BUILD_SCRIPT_PATH="${BUILD_SCRIPT_DIR}/$(basename "$0")"

BUILD_SCRIPT_MAKE_ISO_SCRIPT_PATH="${BUILD_SCRIPT_DIR}/make_iso.bash"

BUILD_SCRIPT_SOURCE_DIR="${BUILD_SCRIPT_DIR}/../.."
BUILD_SCRIPT_BUILD_DIR="${BUILD_SCRIPT_SOURCE_DIR}/build"
BUILD_SCRIPT_CMAKE_TOOLCHAIN_FILE_PATH="${BUILD_SCRIPT_SOURCE_DIR}/alkos/toolchains/${BUILD_SCRIPT_CMAKE_TOOLCHAIN_FILE_NAME}"
BUILD_SCRIPT_CMAKE_LISTS_PATH="${BUILD_SCRIPT_SOURCE_DIR}/alkos/"

BUILD_SCRIPT_OUTPUT_DIR="${BUILD_SCRIPT_BUILD_DIR}/bin"

source "${BUILD_SCRIPT_SOURCE_DIR}/scripts/utils/helpers.bash"
source "${BUILD_SCRIPT_SOURCE_DIR}/scripts/utils/pretty_print.bash"

help() {
  echo "${BUILD_SCRIPT_PATH} [--run | -r] [--verbose | -v]"
  echo "Where:"
  echo "--run     | -r - flag to run the build process of AlkOS"
  echo "--verbose | -v - flag to enable verbose output"
  echo "--release | -rl - flag to build AlkOS in release mode"
}

parse_args() {
  BUILD_SCRIPT_RUN=false
  BUILD_SCRIPT_VERBOSE=false
  BUILD_SCRIPT_BUILD_TYPE="Debug"
  while [[ $# -gt 0 ]]; do
    case $1 in
      -h|--help)
        help
        exit 0
        ;;
      -r|--run)
        BUILD_SCRIPT_RUN=true
        shift
        ;;
      -rl|--release)
        BUILD_SCRIPT_BUILD_TYPE="Release"
        shift
        ;;
      -v|--verbose)
        BUILD_SCRIPT_VERBOSE=true
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
  if [ "$BUILD_SCRIPT_RUN" = false ] ; then
    dump_error "--run flag was not provided!"
    exit 1
  fi
}

main() {
  parse_args "$@"
  process_args

  pretty_info "Building AlkOS"

  mkdir -p "${BUILD_SCRIPT_BUILD_DIR}"

  base_runner "Failed to configure AlkOS" "${BUILD_SCRIPT_VERBOSE}" cmake \
    -B "${BUILD_SCRIPT_BUILD_DIR}" \
    -S "${BUILD_SCRIPT_CMAKE_LISTS_PATH}" \
    -DCMAKE_TOOLCHAIN_FILE="${BUILD_SCRIPT_CMAKE_TOOLCHAIN_FILE_PATH}" \
    -DCMAKE_BUILD_TYPE="${BUILD_SCRIPT_BUILD_TYPE}"
  pretty_success "AlkOS configured successfully"

  base_runner "Failed to build AlkOS" "${BUILD_SCRIPT_VERBOSE}" cmake --build "${BUILD_SCRIPT_BUILD_DIR}"
  base_runner "Failed to create AlkOS ISO" "${BUILD_SCRIPT_VERBOSE}" "${BUILD_SCRIPT_MAKE_ISO_SCRIPT_PATH}" \
   "${BUILD_SCRIPT_BUILD_DIR}/${BUILD_SCRIPT_OUTPUT_DIR}/alkos.iso" "${BUILD_SCRIPT_BUILD_DIR}/sysroot" -v -r
  pretty_success "AlkOS built successfully - Image is located in ${BUILD_SCRIPT_BUILD_DIR}/${BUILD_SCRIPT_OUTPUT_DIR}"
}

main "$@"
