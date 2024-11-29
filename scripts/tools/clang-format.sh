#!/bin/sh

# Get the number of logical CPU cores
num_cores=$(getconf _NPROCESSORS_ONLN)
# Find and format files in parallel using the number of logical CPU cores
find . -regex '\./alkos/.*\.\(cpp\|cc\|h\|c\)$' | xargs -P "$num_cores" -n 1 clang-format -style=file -i
