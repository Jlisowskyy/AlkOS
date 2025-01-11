#!/bin/bash

# Script location setup
readonly ALK_OS_CLI_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
readonly ALK_OS_CLI_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/$(basename "$0")"


# Script paths
readonly ALK_OS_CLI_INSTALL_TOOLCHAIN_PATH="${ALK_OS_CLI_SCRIPT_DIR}/action/install_toolchain.bash"
readonly ALK_OS_CLI_BUILD_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/action/build_alkos.bash"
readonly ALK_OS_CLI_INSTALL_DEPS_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/env/install_deps_arch.bash"
readonly ALK_OS_CLI_QEMU_RUN_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/action/run_alkos.bash"
readonly ALK_OS_CLI_CONFIGURE_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/configure.bash"
readonly ALK_OS_CLI_CONF_PATH="${ALK_OS_CLI_SCRIPT_DIR}/conf.bash"

# Import utilities
source "${ALK_OS_CLI_SCRIPT_DIR}/utils/pretty_print.bash"
source "${ALK_OS_CLI_SCRIPT_DIR}/utils/helpers.bash"

# Default configuration
declare -A CONFIG=(
    [run]=false
    [install_toolchain]=false
    [install_deps]=false
    [verbose]=false
    [configure]=false
)

print_banner() {
    cat << "EOF"
    _    _ _    _____ _____
   / \  | | | _|  _  |   __|
  / _ \ | | |/ / | | |__   |
 / ___ \| |   <| |_| |     |
/_/   \_\_|_|\_\_____|_____| CLI
EOF
    echo "AlkOS Environment Setup Tool"
    echo "Version: 0.0.0"
    echo
}

help() {
    cat << EOF
Usage: ${ALK_OS_CLI_SCRIPT_PATH} [options]

Options:
    -c, --configure             Run default configuration [x86_64]
    -r, --run                    Build and run AlkOS
    -i, --install <component>    Install components:
                                - toolchain: Cross-compiler toolchain
                                - deps: System dependencies
                                - all: Both toolchain and dependencies
    -v, --verbose               Enable verbose output
    -h, --help                  Display this help message

Examples:
    ${0##*/} --install all              # Install all dependencies
    ${0##*/} --arch x86_64 --run        # Build and run for x86_64
    ${0##*/} --dev --run                # Build and run in development mode

EOF
}

parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                help
                exit 0
                ;;
            -r|--run)
                CONFIG[run]=true
                shift
                ;;
            -i|--install)
                case $2 in
                    toolchain)
                        CONFIG[install_toolchain]=true
                        ;;
                    deps)
                        CONFIG[install_deps]=true
                        ;;
                    all)
                        CONFIG[install_toolchain]=true
                        CONFIG[install_deps]=true
                        ;;
                    *)
                        dump_error "Invalid install component: $2"
                        exit 1
                        ;;
                esac
                shift 2
                ;;
            -v|--verbose)
                CONFIG[verbose]=true
                shift
                ;;
            -c|--configure)
                CONFIG[configure]=true
                shift
                ;;
            *)
                dump_error "Unknown argument: $1"
                exit 1
                ;;
        esac
    done
}

validate_args() {
    if [[ ${CONFIG[run]} == false ]] &&
       [[ ${CONFIG[install_toolchain]} == false ]] &&
       [[ ${CONFIG[install_deps]} == false ]] &&
       [[ ${CONFIG[configure]} == false ]]; then
        dump_error "No action specified. Use --run, --install or --configure"
        exit 1
    fi

    if [[ ! -d "$ALK_OS_CLI_DEFAULT_TOOL_INSTALL_DIR" ]]; then
        mkdir -p "$ALK_OS_CLI_DEFAULT_TOOL_INSTALL_DIR"
    fi

    if [[ ! -d "$ALK_OS_CLI_DEFAULT_BUILD_DIR" ]]; then
        mkdir -p "$ALK_OS_CLI_DEFAULT_BUILD_DIR"
    fi
}

run_default_configuration() {
    if [[ ${CONFIG[configure]} == true ]]; then
        pretty_info "Running default configuration"
        base_runner "Failed to run default configuration" true \
            "${ALK_OS_CLI_CONFIGURE_SCRIPT_PATH}" x86_64 debug_qemu ${ALK_OS_CLI_VERBOSE_FLAG}
    fi
}

install_dependencies() {
    if [[ ${CONFIG[install_deps]} == true ]]; then
        pretty_info "Installing system dependencies"
        check_runs_on_arch
        if [[ $? -ne 0 ]]; then
            dump_error "Dependencies installation only supported on Arch Linux"
            exit 1
        fi

        base_runner "Failed to install dependencies" true \
            "${ALK_OS_CLI_INSTALL_DEPS_SCRIPT_PATH}" --install ${ALK_OS_CLI_VERBOSE_FLAG}
    fi
}

validate_configuration_exists() {
  if [ ! -f "${ALK_OS_CLI_CONF_PATH}" ] ; then
     dump_error "Configuration must be ran before proceeding to the build. Use configure.bash script to configure the environment"
    exit 1
  fi
}

install_toolchain() {
    if [[ ${CONFIG[install_toolchain]} == true ]]; then
        validate_configuration_exists

        pretty_info "Installing cross-compile toolchain for ${CONFIG[arch]}"
        base_runner "Failed to install cross-compile toolchain" true \
            "${ALK_OS_CLI_INSTALL_TOOLCHAIN_PATH}" ${ALK_OS_CLI_VERBOSE_FLAG}
    fi
}

build_and_run() {
    if [[ ${CONFIG[run]} == true ]]; then
        validate_configuration_exists

        pretty_info "Building AlkOS for ${CONFIG[arch]}"
        base_runner "Failed to build AlkOS" true "${ALK_OS_CLI_BUILD_SCRIPT_PATH}" ${ALK_OS_CLI_VERBOSE_FLAG}

        pretty_info "Running AlkOS in QEMU"
        base_runner "Failed to run AlkOS in QEMU" true "${ALK_OS_CLI_QEMU_RUN_SCRIPT_PATH}" ${ALK_OS_CLI_VERBOSE_FLAG}
    fi
}

main() {
    print_banner
    parse_args "$@"
    validate_args
    install_dependencies
    run_default_configuration
    install_toolchain
    build_and_run
}

main "$@"