#!/bin/bash

CHECK_CLANG_FORMAT_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CHECK_CLANG_FORMAT_FORMAT_FILE="${CHECK_CLANG_FORMAT_SCRIPT_DIR}/../../.clang-format"

declare -a DIRS=(
    "${CHECK_CLANG_FORMAT_SCRIPT_DIR}/../../alkos/kernel"
    "${CHECK_CLANG_FORMAT_SCRIPT_DIR}/../../alkos/libc"
)

find_files() {
  local files=()
  for dir in "${DIRS[@]}"; do
    files+=($(find "$dir" -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.h" -o -iname "*.hpp" \)))
  done
  echo "${files[@]}"
}

check_clang_format() {
  local files=("$@")

  echo "Checking clang-format using $(nproc) CPU cores..."

  parallel -j $(nproc) --halt now,fail=1 clang-format -style=file --dry-run -Werror ::: "${files[@]}"

  if [ $? -ne 0 ]; then
    echo "Some files are not correctly clang-formatted."
    exit 1
  else
    echo "All files are correctly clang-formatted."
  fi
}

main() {
  echo "Checking clang-format..."

  files_to_check=$(find_files)

  if [ -z "$files_to_check" ]; then
    echo "No source files found to check."
    exit 0
  fi

  check_clang_format $files_to_check
}

main
