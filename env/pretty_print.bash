#!/bin/bash

# ANSI color codes
RED='\033[0;31m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Pretty echo function
pretty_echo() {
    local type=$1
    shift
    local message="$@"

    case "${type,,}" in # Convert to lowercase for comparison
        "error")
            echo -e "${RED}${BOLD}[ERROR]${NC} ${message}"
            ;;
        "warning")
            echo -e "${YELLOW}${BOLD}[WARNING]${NC} ${message}"
            ;;
        "info")
            echo -e "${BLUE}${BOLD}[INFO]${NC} ${message}"
            ;;
        *)
            echo -e "${GREEN}${BOLD}[DEBUG]${NC} ${message}"
            ;;
    esac
}

pretty_info() {
  pretty_echo "info" "$@"
}

pretty_error() {
  pretty_echo "error" "$@"
}

pretty_warn() {
  pretty_echo "warning" "$@"
}

pretty_debug() {
  pretty_echo "" "$@"
}
