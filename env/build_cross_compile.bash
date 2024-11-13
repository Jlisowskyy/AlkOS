#!/bin/bash

SCRIPT_DIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
BIN_UTILS_VER="2.43.1"
PROC_COUNT=$(nproc --all)
TARGET=i686-elf

source "${SCRIPT_DIR}/pretty_print.bash"
source "${SCRIPT_DIR}/helpers.bash"

help() {
  echo "${SCRIPT_PATH} --install [--build_dir <dir>] [--tool_dir <dir>]"
  echo "Where:"
  echo "--install         - required flag to start installation"
  echo "--build_dir <dir> - provides directory <dir> to save all build files"
  echo "--tool_dir  <dir> - directory where tooling should be saved"
}

prepare_directory() {
  assert_no_argument "$1"
  local dir="$1"

  if ! [ -d "${dir}" ]; then
    pretty_info "Creating directory ${dir}"
    mkdir -p "${dir}" || dump_error "Failed to create directory ${dir}"
  else
    pretty_info "Directory ${dir} already exists"
  fi
}

download_source() {
  assert_no_argument "$1"
  assert_no_argument "$2"

  local name="$1"
  local link="$2"

  if ! [ -f "${name}" ]; then
    pretty_info "Downloading ${name}"
    wget "${link}" || dump_error "Failed to download ${name}"
  else
    pretty_info "${name} already downloaded"
  fi
}

download_extract_binutils_source() {
  assert_no_argument "$1"
  local name="$1"

  download_source "${name}" "https://ftp.gnu.org/gnu/binutils/${name}"

  pretty_info "Extracting ${name}"
  tar -xf "${name}" || dump_error "Failed to extract ${name}"
}

build_binutils() {
    local binutils_dir="${BUILD_DIR}/build_binutils"
    local binutils_name="binutils-${BIN_UTILS_VER}"

    pretty_info "Building binutils"

    prepare_directory "${binutils_dir}"
    cd "${binutils_dir}"

    download_extract_binutils_source "${binutils_name}.tar.gz"

    pretty_info "Configuring binutils"
    ${binutils_name}/configure --target="${TARGET}" --prefix="${TOOL_DIR}" --with-sysroot --disable-nls --disable-werror || dump_error "Failed to configure binutils"

    pretty_info "Building binutils with ${PROC_COUNT} threads"
    make -j "${PROC_COUNT}" || dump_error "Failed to build binutils"

    pretty_info "Installing binutils"
    make install || dump_error "Failed to install binutils"
}

build_gcc() {
  echo ELO
}

build_gdb() {
  local gdb_dir="${BUILD_DIR}/build_gdb"
  local gdb_name="gdb-15.2"

  pretty_info "Building GDB"

  prepare_directory "${gdb_dir}"
  cd "${gdb_dir}"

  download_extract_binutils_source "${gdb_name}.tar.gz"

  pretty_info "Configuring GDB"
  ../${gdb_name}/configure --target=${TARGET} --prefix="${TOOL_DIR}" --disable-werror

  pretty_info "Building GDB with ${PROC_COUNT} threads"
  make -j "${PROC_COUNT}" all-gdb

  pretty_info "Installing GDB"
  make install-gdb
}

run_build() {
  #    export PATH="$TOOL_DIR/bin:$PATH"

  pretty_info "Starting GCC cross-compiler build with build directory: ${BUILD_DIR} and target directory: ${TOOL_DIR}"

  build_binutils
  build_gdb
  build_gcc
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
        dump_error "Unknown option $1"
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
  parse_args "$@"
  process_args
  run_build
}

main "$@"