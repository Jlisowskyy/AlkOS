#!/bin/bash

SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
SCRIPT_PATH="${SCRIPT_DIR}/$(basename "$0")"
DEFAULT_SYS_INSTALL_DIR="${SCRIPT_DIR}/../sys_root"
DEFAULT_TOOL_INSTALL_DIR="${SCRIPT_DIR}/../tools"
DEFAULT_BUILD_DIR="${SCRIPT_DIR}/../build"

source "${SCRIPT_DIR}/pretty_print.bash"

help() {
  echo "${SCRIPT_PATH} --install [--build_dir <dir>] [--tool_dir <dir>] [--sys_root <dir>]"
  echo "Where:"
  echo "--install         - required flag to start installation"
  echo "--build_dir <dir> - provides directory <dir> to save all build files"
  echo "--tool_dir  <dir> - directory where tooling should be saved"
  echo "--sys_root  <dir> - directory serving as a base dir for the system"
  echo
  echo "Note: directory in which script is stored will be used as base for default paths"
}

dump_error() {
  help

  pretty_error $1
  exit 1
}

run_build() {
  "${SCRIPT_DIR}/build_cross_compile.bash" --install --build_dir BUILD_DIR --tool_dir TOOL_DIR
}

parse_args() {
  POSITIONAL_ARGS=()
  INSTALL_FOUND=false

  while [[ $# -gt 0 ]]; do
    case $1 in
      --tool_dir)
        TOOL_DIR="$2"
        shift
        shift
        ;;
      --sys_root)
        SYS_DIR="$2"
        shift
        shift
        ;;
      --build_dir)
        BUILD_DIR="$2"
        shift
        shift
        ;;
      -h|--help)
        help
        exit 0
        ;;
      --install)
        INSTALL_FOUND=true
        shift
        ;;
      -*|--*)
        dump_error "Unknown option $i"
        ;;
      *)
        POSITIONAL_ARGS+=("$1")
        shift
        ;;
    esac
  done

  set -- "${POSITIONAL_ARGS[@]}"
}

process_args() {
  if [ $INSTALL_FOUND = false ] ; then
    dump_error "--install flag was not provided!"
  fi

  if [ -z "${TOOL_DIR}" ] ; then
    TOOL_DIR="$DEFAULT_TOOL_INSTALL_DIR"
  fi

  if [ -z "${BUILD_DIR}" ] ; then
    BUILD_DIR="$DEFAULT_BUILD_DIR"
  fi

  if [ -z "${SYS_DIR}" ] ; then
    SYS_DIR="$DEFAULT_SYS_INSTALL_DIR"
  fi
}

main() {
  parse_args $@
  process_args
  run_build
}

main $@