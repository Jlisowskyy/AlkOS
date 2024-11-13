#!/bin/bash

pretty_echo() {
    local type=$1
    shift
    local message="$@"

    local red='\033[0;31m'
    local yellow='\033[1;33m'
    local green='\033[0;32m'
    local blue='\033[0;34m'
    local cyan='\033[0;36m'
    local nc='\033[0m'
    local bold='\033[1m'

    case "${type,,}" in
        "error")
            echo -e "${red}${bold}[ERROR]${nc} ${message}"
            ;;
        "warning")
            echo -e "${yellow}${bold}[WARNING]${nc} ${message}"
            ;;
        "info")
            echo -e "${blue}${bold}[INFO]${nc} ${message}"
            ;;
        "success")
            echo -e "${green}${bold}[SUCCESS]${nc} ${message}"
            ;;
        "debug")
            echo -e "${cyan}${bold}[DEBUG]${nc} ${message}"
            ;;
        *)
            echo -e "${red}${bold}[ERROR]${nc} Unknown message type in pretty print: ${type}!"
            exit 1
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

pretty_success() {
    pretty_echo "success" "$@"
}

pretty_debug() {
    pretty_echo "debug" "$@"
}
