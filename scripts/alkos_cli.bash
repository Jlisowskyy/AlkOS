#!/bin/bash

ALK_OS_CLI_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
ALK_OS_CLI_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/$(basename "$0")"

ALK_OS_CLI_DEFAULT_TOOL_INSTALL_DIR="${ALK_OS_CLI_SCRIPT_DIR}/../tools"
ALK_OS_CLI_DEFAULT_BUILD_DIR="${ALK_OS_CLI_SCRIPT_DIR}/../build"

ALK_OS_CLI_INSTALL_TOOLCHAIN_PATH="${ALK_OS_CLI_SCRIPT_DIR}/env/build_cross_compile.bash"
ALK_OS_CLI_BUILD_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/install/build_alkos.bash"
ALK_OS_CLI_INSTALL_DEPS_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/env/install_deps_arch.bash"
ALK_OS_CLI_QEMU_RUN_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/install/run_alkos.bash"
ALK_OS_CLI_ISO_PATH="${ALK_OS_CLI_DEFAULT_BUILD_DIR}/bin/alkos.iso"

source "${ALK_OS_CLI_SCRIPT_DIR}/utils/pretty_print.bash"
source "${ALK_OS_CLI_SCRIPT_DIR}/utils/helpers.bash"

help() {
  echo "${ALK_OS_CLI_SCRIPT_PATH} [--run | -r] [--install | -i] [--verbose | -v]"
  echo "Where:"
  echo "--run     | -r - flag to run the build and deployment process of AlkOS"
  echo "--install [toolchain | deps | all] | -i [toolchain | deps | all] - flag to install toolchain or dependencies"
  echo "--verbose | -v - flag to enable verbose output"
  echo "Note: If both --run and --install flags are provided, the script will first
  install dependencies and cross-compile toolchain, then run the build and deployment process"
}

parse_args() {
  ALK_OS_CLI_RUN=false
  ALK_OS_CLI_INSTALL_TOOLCHAIN=false
  ALK_OS_CLI_INSTALL_DEPS=false
  ALK_OS_CLI_VERBOSE=false
  while [[ $# -gt 0 ]]; do
    case $1 in
      -h|--help)
        help
        exit 0
        ;;
      -r|--run)
        ALK_OS_CLI_RUN=true
        shift
        ;;
      -i|--install)
        case $2 in
          toolchain)
            ALK_OS_CLI_INSTALL_TOOLCHAIN=true
            ;;
          deps)
            ALK_OS_CLI_INSTALL_DEPS=true
            ;;
          all)
            ALK_OS_CLI_INSTALL_TOOLCHAIN=true
            ALK_OS_CLI_INSTALL_DEPS=true
            ;;
          *)
            dump_error "Unknown argument for --install flag: $2"
            ;;
        esac
        shift
        shift
        ;;
      -v|--verbose)
        ALK_OS_CLI_VERBOSE=true
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
  if [ $ALK_OS_CLI_RUN = false ] && [ $ALK_OS_CLI_INSTALL_TOOLCHAIN = false ] && [ $ALK_OS_CLI_INSTALL_DEPS = false ] ; then
    dump_error "No action provided! Please provide either --run or --install flag"
  fi
  ALK_OS_CLI_VERBOSE_FLAG=""
  if [ $ALK_OS_CLI_VERBOSE = true ] ; then
    ALK_OS_CLI_VERBOSE_FLAG="--verbose"
  fi
}

main() {
  parse_args "$@"
  process_args

  if [ $ALK_OS_CLI_INSTALL_DEPS = true ] ; then
    pretty_info "Installing dependencies"
    check_runs_on_arch
    local runs_on_arch=$?
    if [ $runs_on_arch -ne 0 ]; then
      dump_error "Installation of dependencies is only supported on Arch Linux"
    fi
    ALK_OS_CLI_VERBOSE=true
    base_runner "Failed to install dependencies" true "${ALK_OS_CLI_INSTALL_DEPS_SCRIPT_PATH}" --install ${ALK_OS_CLI_VERBOSE_FLAG}
  fi

  if [ $ALK_OS_CLI_INSTALL_TOOLCHAIN = true ] ; then
    pretty_info "Installing cross-compile toolchain"
    base_runner "Failed to install cross-compile toolchain" true "${ALK_OS_CLI_INSTALL_TOOLCHAIN_PATH}" --install \
      toolchain ${ALK_OS_CLI_VERBOSE_FLAG} -t "${ALK_OS_CLI_DEFAULT_TOOL_INSTALL_DIR}" -b "${ALK_OS_CLI_DEFAULT_BUILD_DIR}"
  fi

  if [ $ALK_OS_CLI_RUN = true ] ; then
    base_runner "Failed to build AlkOS" "${ALK_OS_CLI_VERBOSE}" "${ALK_OS_CLI_BUILD_SCRIPT_PATH}" --run ${ALK_OS_CLI_VERBOSE_FLAG}
    base_runner "Failed to run AlkOS in QEMU" "${ALK_OS_CLI_VERBOSE}" "${ALK_OS_CLI_QEMU_RUN_SCRIPT_PATH}" "${ALK_OS_CLI_ISO_PATH}" --run ${ALK_OS_CLI_VERBOSE_FLAG}
  fi
}

main "$@"