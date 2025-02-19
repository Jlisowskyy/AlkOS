#!/bin/bash

# Script location setup
readonly ALK_OS_CLI_SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
readonly ALK_OS_CLI_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/$(basename "$0")"


# Script paths
readonly ALK_OS_CLI_INSTALL_TOOLCHAIN_PATH="${ALK_OS_CLI_SCRIPT_DIR}/actions/install_toolchain.bash"
readonly ALK_OS_CLI_BUILD_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/actions/build_alkos.bash"
readonly ALK_OS_CLI_INSTALL_DEPS_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/env/install_deps_arch.bash"
readonly ALK_OS_CLI_QEMU_RUN_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/actions/run_alkos.bash"
readonly ALK_OS_CLI_CONFIGURE_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/configure.bash"
readonly ALK_OS_CLI_CONF_PATH="${ALK_OS_CLI_SCRIPT_DIR}/conf.bash"
readonly ALK_OS_CLI_SETUP_HOOKS_SCRIPT_PATH="${ALK_OS_CLI_SCRIPT_DIR}/git-hooks/setup-hooks.bash"

# Import utilities
source "${ALK_OS_CLI_SCRIPT_DIR}/utils/pretty_print.bash"
source "${ALK_OS_CLI_SCRIPT_DIR}/utils/helpers.bash"

# Default configuration
declare -A CONFIG=(
    [run]=false
    [install_toolchain]=false
    [install_deps]=false
    [verbose]=""
    [configure]=false
    [setup_hooks]=false
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
    -r, --run                   Build and run AlkOS
    -i, --install <component>   Install components:
                                - toolchain: Cross-compiler toolchain
                                - deps: System dependencies
                                - all: Both toolchain and dependencies
    -v, --verbose               Enable verbose output
    -h, --help                  Display this help message
    -g, --git-hooks             Setup git hooks

Examples:
    ${0##*/} --configure --install all              # Configure and install all dependencies

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
                CONFIG[verbose]="-v"
                shift
                ;;
            -c|--configure)
                CONFIG[configure]=true
                shift
                ;;
            -g|--git-hooks)
                CONFIG[setup_hooks]=true
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
       [[ ${CONFIG[configure]} == false ]] &&
       [[ ${CONFIG[setup_hooks]} == false ]]; then
        dump_error "No action specified. Use --run, --install, --configure or --git-hooks."
        exit 1
    fi
}

run_default_configuration() {
    if [[ ${CONFIG[configure]} == true ]]; then
        pretty_info "Running default configuration"
        base_runner "Failed to run default configuration" true \
            "${ALK_OS_CLI_CONFIGURE_SCRIPT_PATH}" x86_64 debug_qemu ${CONFIG[verbose]}
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
            "${ALK_OS_CLI_INSTALL_DEPS_SCRIPT_PATH}" --install ${CONFIG[verbose]}
    fi
}

validate_configuration_exists() {
  if [ ! -f "${ALK_OS_CLI_CONF_PATH}" ] ; then
     dump_error "Coudn't find configuration. Use configure.bash script to configure the environment"
    exit 1
  fi
}

install_toolchain() {
    if [[ ${CONFIG[install_toolchain]} == true ]]; then
        validate_configuration_exists

        pretty_info "Installing cross-compile toolchain for ${CONFIG[arch]}"
        base_runner "Failed to install cross-compile toolchain" true \
            "${ALK_OS_CLI_INSTALL_TOOLCHAIN_PATH}" ${CONFIG[verbose]}
    fi
}

build_and_run() {
    if [[ ${CONFIG[run]} == true ]]; then
        validate_configuration_exists

        pretty_info "Building AlkOS..."
        base_runner "Failed to build AlkOS" true "${ALK_OS_CLI_BUILD_SCRIPT_PATH}" ${CONFIG[verbose]}

        pretty_info "Running AlkOS in QEMU"
        base_runner "Failed to run AlkOS in QEMU" true "${ALK_OS_CLI_QEMU_RUN_SCRIPT_PATH}" --verbose
    fi
}

setup_git_hooks() {
  if [[ ${CONFIG[setup_hooks]} == true ]]; then
    base_runner "Failed to setup git-hooks" true "${ALK_OS_CLI_SETUP_HOOKS_SCRIPT_PATH}"
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
    setup_git_hooks
}

main "$@"

