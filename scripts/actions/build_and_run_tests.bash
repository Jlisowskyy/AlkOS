#!/bin/bash

BUILD_AND_RUN_TESTS_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Step 1: Get requirements
"${BUILD_AND_RUN_TESTS_DIR}/../env/install_deps_arch.bash" --install -v

# Step 2: Configure the environment
# TODO: temporarly x86_64 is hardcoded
"${BUILD_AND_RUN_TESTS_DIR}/../install/configure.bash" x86_64 debug_qemu_tests -v

# Step 3: Build toolchain
"${BUILD_AND_RUN_TESTS_DIR}/../actions/install_toolchain.bash" -v

# Step 4: Build the project
"${BUILD_AND_RUN_TESTS_DIR}/../actions/build_alkos.bash" -v

# Step 5: Run tests
"${BUILD_AND_RUN_TESTS_DIR}/../actions/run_tests.bash"
