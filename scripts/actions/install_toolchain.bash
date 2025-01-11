INSTALL_TOOLCHAIN_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"

INSTALL_TOOLCHAIN_DEFAULT_ARCHES=("x86_64-elf" "i386-elf") # TODO: read from config?

INSTALL_TOOLCHAIN_INSTALL_DIR="${INSTALL_TOOLCHAIN_DIR}/../../tools"
INSTALL_TOOLCHAIN_BUILD_DIR="${INSTALL_TOOLCHAIN_DIR}/../../build"

INSTALL_TOOLCHAIN_BUILD_SCRIPT_PATH="${INSTALL_TOOLCHAIN_DIR}/../env/build_cross_compile.bash"
INSTALL_TOOLCHAIN_CLEAN_BUILD_SCRIPT_PATH="${INSTALL_TOOLCHAIN_DIR}../install/clean_build_dir.bash"

source "${INSTALL_TOOLCHAIN_DIR}/../utils/pretty_print.bash"
source "${INSTALL_TOOLCHAIN_DIR}/../utils/helpers.bash"

help() {
  echo "${INSTALL_TOOLCHAIN_SCRIPT_PATH} [--verbose | -v]"
  echo "Where:"
  echo "--verbose | -v - flag to enable verbose output"
}

parse_args() {
  INSTALL_TOOLCHAIN_VERBOSE=false
  while [[ $# -gt 0 ]]; do
    case $1 in
      -h|--help)
        help
        exit 0
        ;;
      -v|--verbose)
        INSTALL_TOOLCHAIN_VERBOSE=true
        shift
        ;;
      *)
        dump_error "Unknown argument: $1"
        ;;
    esac
  done
}

process_args() {
  if [[ "${INSTALL_TOOLCHAIN_VERBOSE}" == "true" ]]; then
    INSTALL_TOOLCHAIN_VERBOSE_FLAG="--verbose"
  else
    INSTALL_TOOLCHAIN_VERBOSE_FLAG=""
  fi
}

main() {
  parse_args "$@"
  process_args

  pretty_info "Installing cross-compile toolchain"
  for arch in "${INSTALL_TOOLCHAIN_DEFAULT_ARCHES[@]}"; do
    pretty_info "Installing for ${arch}"
    base_runner "Failed to clean build directory" true "${INSTALL_TOOLCHAIN_CLEAN_BUILD_SCRIPT_PATH}" ${INSTALL_TOOLCHAIN_VERBOSE_FLAG}
    # Spawn a subshell to run the build script (to avoid polluting the current shell)
    (
      base_runner "Failed to install cross-compile toolchain" true "${INSTALL_TOOLCHAIN_BUILD_SCRIPT_PATH}" --install \
        toolchain "${INSTALL_TOOLCHAIN_VERBOSE_FLAG}"   \
          -t "${INSTALL_TOOLCHAIN_INSTALL_DIR}/${arch}" \
          -b "${INSTALL_TOOLCHAIN_BUILD_DIR}/${arch}"   \
          -c "${arch}"

      exit 0
    ) 2>&1 || exit 1
  done
}

main "$@"

