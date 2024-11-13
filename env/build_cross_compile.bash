#!/bin/bash

SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

source "${SCRIPT_DIR}/pretty_print.bash"

help() {
  echo "${SCRIPT_PATH} --install [--build_dir <dir>] [--tool_dir <dir>]"
  echo "Where:"
  echo "--install         - required flag to start installation"
  echo "--build_dir <dir> - provides directory <dir> to save all build files"
  echo "--tool_dir  <dir> - directory where tooling should be saved"
}

dump_error() {
  help

  pretty_error $1
  exit 1
}

run_build() {

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
    dump_error "--tool_dir flag with directory where save tooling was not provided!"
  fi

  if [ -z "${BUILD_DIR}" ] ; then
    dump_error "--build_dir flag with directory where save build files was not provided!"
  fi
}

main() {
  parse_args $@
  process_args
  run_build
}

main $@