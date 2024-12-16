#!/bin/bash
main() {
  local filename="$1"
  local objcopy_cmd="$2"
  echo "Converting $filename to ELF64 format..."

  if [ ! -f "$filename" ]; then
    echo "File not found: $filename"
    exit 1
  fi

  if file "$filename" | grep -q "ELF 64-bit"; then
    echo "File is already in ELF64 format"
    exit 0
  fi

  echo "Converting $filename to ELF64 format..."
  ${objcopy_cmd} -I elf32-i386 -O elf64-x86-64 "$filename" "$filename"
}

main "$@"
