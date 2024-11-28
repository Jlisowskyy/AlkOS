#!/bin/bash

DEPLOY_SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
DEPLOY_SCRIPT_PATH="${DEPLOY_SCRIPT_DIR}/$(basename "$0")"

INSTALL_TOOLCHAIN_PATH="${DEPLOY_SCRIPT_DIR}/env/install.bash"
BUILD_SCRIPT_PATH="${DEPLOY_SCRIPT_DIR}/scripts/build_alkos.bash"
INSTALL_DEPS_SCRIPT_PATH="${DEPLOY_SCRIPT_DIR}/env/install_deps_arch.bash"
QEMU_RUN_SCRIPT_PATH="${DEPLOY_SCRIPT_DIR}/scripts/run_alkos.bash"

source "${DEPLOY_SCRIPT_DIR}/env/pretty_print.bash"
source "${DEPLOY_SCRIPT_DIR}/env/helpers.bash"

help() {
  echo "${DEPLOY_SCRIPT_PATH} [--run | -r] [--install | -i] [--verbose | -v]"
  echo "Where:"
  echo "--run     | -r - flag to run the build and deployment process of AlkOS"
  echo "--install [toolchain | deps | all] | -i [toolchain | deps | all] - flag to install toolchain or dependencies"
  echo "--verbose | -v - flag to enable verbose output"
  echo "Note: If both --run and --install flags are provided, the script will first
  install dependencies and cross-compile toolchain, then run the build and deployment process"
}

parse_args() {
  RUN=false
  INSTALL_TOOLCHAIN=false
  INSTALL_DEPS=false
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
      -i|--install)
        case $2 in
          toolchain)
            INSTALL_TOOLCHAIN=true
            ;;
          deps)
            INSTALL_DEPS=true
            ;;
          all)
            INSTALL_TOOLCHAIN=true
            INSTALL_DEPS=true
            ;;
          *)
            dump_error "Unknown argument for --install flag: $2"
            ;;
        esac
        shift
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
  if [ $RUN = false ] && [ $INSTALL_TOOLCHAIN = false ] && [ $INSTALL_DEPS = false ] ; then
    dump_error "No action provided! Please provide either --run or --install flag"
  fi
  VERBOSE_FLAG=""
  if [ $VERBOSE = true ] ; then
    VERBOSE_FLAG="--verbose"
  fi
}

main() {
  parse_args "$@"
  process_args

  if [ $INSTALL_TOOLCHAIN = true ] ; then
    pretty_info "Installing cross-compile toolchain"
    base_runner "Failed to install cross-compile toolchain" "${VERBOSE}" "${INSTALL_TOOLCHAIN_PATH}" --install toolchain ${VERBOSE_FLAG}
  fi

  if [ $INSTALL_DEPS = true ] ; then
    pretty_info "Installing dependencies"
    check_runs_on_arch
    local runs_on_arch=$?
    if [ $runs_on_arch -ne 0 ]; then
      dump_error "Installation of dependencies is only supported on Arch Linux"
    fi
    VERBOSE=true
    base_runner "Failed to install dependencies" "${VERBOSE}" "${INSTALL_DEPS_SCRIPT_PATH}" --install ${VERBOSE_FLAG}
  fi

  if [ $RUN = true ] ; then
    # shellcheck disable=SC2086
    base_runner "Failed to build AlkOS" "${VERBOSE}" ${BUILD_SCRIPT_PATH} --run ${VERBOSE_FLAG}
    base_runner "Failed to run AlkOS in QEMU" "${VERBOSE}" "${QEMU_RUN_SCRIPT_PATH}" --run ${VERBOSE_FLAG}
  fi
}

main "$@"