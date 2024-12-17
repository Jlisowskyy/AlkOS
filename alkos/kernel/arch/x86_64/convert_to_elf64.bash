#!/bin/bash

OBJCOPY_CMD="objcopy"

convert_file() {
  local filename="$1"
  echo "Converting $filename to ELF64 format..."

  if [ ! -f "$filename" ]; then
    echo "File not found: $filename"
    return
  fi

  if file "$filename" | grep -q "ELF 64-bit"; then
    echo "File is already in ELF64 format"
    return
  fi

  echo "Converting $filename to ELF64 format..."
  ${OBJCOPY_CMD} -I elf32-i386 -O elf64-x86-64 "$filename" "$filename"
}
main() {
  OBJCOPY_CMD=$1

  # split second argument by ';'
  IFS=';' read -r -a files <<< "$2"

  for file in "${files[@]}"; do
    convert_file "$file"
  done
}

main "$@"
