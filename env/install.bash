#!/bin/bash

INSTALL_SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
INSTALL_SCRIPT_PATH="${INSTALL_SCRIPT_DIR}/$(basename "$0")"

INSTALL_DEFAULT_SYS_INSTALL_DIR="${INSTALL_SCRIPT_DIR}/../sys_root"
INSTALL_DEFAULT_TOOL_INSTALL_DIR="${INSTALL_SCRIPT_DIR}/../tools"
INSTALL_DEFAULT_BUILD_DIR="${INSTALL_SCRIPT_DIR}/../build"

INSTALL_POSITIONAL_ARGS=()
INSTALL_FOUND=false
INSTALL_VERBOSE=""

source "${INSTALL_SCRIPT_DIR}/pretty_print.bash"
source "${INSTALL_SCRIPT_DIR}/helpers.bash"

help() {
  echo "${INSTALL_SCRIPT_PATH} --install [--build_dir <dir>] [--tool_dir <dir>] [--sys_root <dir>] [-v | --verbose]"
  echo "Where:"
  echo "--install         | -i       - required flag to start installation"
  echo "--build_dir <dir> | -b <dir> - provides directory <dir> to save all build files"
  echo "--tool_dir  <dir> | -t <dir> - directory where tooling should be saved"
  echo "--sys_root  <dir> | -s <dir> - directory serving as a base dir for the system"
  echo "--verbose         | -v       - flag to enable verbose output"
  echo
  echo "Note: directory in which script is stored will be used as base for default paths"
}

run_build() {
  "${INSTALL_SCRIPT_DIR}/build_cross_compile.bash" --install --build_dir "${INSTALL_BUILD_DIR}" --tool_dir "${INSTALL_TOOL_DIR}" "${INSTALL_VERBOSE}"
}

parse_args() {
  while [[ $# -gt 0 ]]; do
    case $1 in
      -t|--tool_dir)
        INSTALL_TOOL_DIR="$2"
        shift
        shift
        ;;
      -s|--sys_root)
        INSTALL_SYS_DIR="$2"
        shift
        shift
        ;;
      -b|--build_dir)
        INSTALL_BUILD_DIR="$2"
        shift
        shift
        ;;
      -h|--help)
        help
        exit 0
        ;;
      -i|--install)
        INSTALL_FOUND=true
        shift
        ;;
      -v|--verbose)
        INSTALL_VERBOSE="--verbose"
        shift
        ;;
      -*|--*)
        dump_error "Unknown option $i"
        ;;
      *)
        INSTALL_POSITIONAL_ARGS+=("$1")
        shift
        ;;
    esac
  done

  set -- "${INSTALL_POSITIONAL_ARGS[@]}"
}

process_args() {
  if [ $INSTALL_FOUND = false ] ; then
    dump_error "--install flag was not provided!"
  fi

  if [ -z "${INSTALL_TOOL_DIR}" ] ; then
    INSTALL_TOOL_DIR="$INSTALL_DEFAULT_TOOL_INSTALL_DIR"
  fi

  if [ -z "${INSTALL_BUILD_DIR}" ] ; then
    INSTALL_BUILD_DIR="$INSTALL_DEFAULT_BUILD_DIR"
  fi

  if [ -z "${INSTALL_SYS_DIR}" ] ; then
    INSTALL_SYS_DIR="$INSTALL_DEFAULT_SYS_INSTALL_DIR"
  fi
}

main() {
  parse_args "$@"
  process_args
  run_build
}

main "$@"
