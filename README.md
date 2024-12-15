# AlkOS
[![License](https://img.shields.io/github/license/Jlisowskyy/AlkOS)](https://github.com/Jlisowskyy/AlkOS/blob/main/LICENSE)
[![Stars](https://img.shields.io/github/stars/Jlisowskyy/AlkOS)](https://github.com/Jlisowskyy/AlkOS/stargazers)
[![Issues](https://img.shields.io/github/issues/Jlisowskyy/AlkOS)](https://github.com/Jlisowskyy/AlkOS/issues)
[![Contributors](https://img.shields.io/github/contributors/Jlisowskyy/AlkOS)](https://github.com/Jlisowskyy/AlkOS/graphs/contributors)

AlkOS is an aspiring open-source x86_64 operating system project. It's currently in it's infancy stages and under heavy development.

## 📁 Directory Layout

- **scripts/**
    - `env/` - Scripts for preparing, updating, and modifying the development and user environments.
    - `utils/` - Utility scripts to be sourced and used by other scripts.
    - `install/` - Scripts for performing user operations like preparing ISO, compiling user versions, or installing the OS.
    - `tests/` - Scripts used to automatically test the project.
    - `actions/` - Scripts for performing useful operations on the repository.

## 🛠️ Key Scripts

- `alkos_cli.bash` - Performs modular installation of the OS, including cross-compiling, setting up the development environment, compiling binaries, building the ISO, and preparing bootable devices. It primarily invokes other scripts.
- `actions/prepare_env.bash` - Performs a full installation of the development environment, including debug installations.

## Getting Started

### 📋 Prerequisites

- Arch Linux (for installing dependencies)
- CMake >= 3.30
- QEMU
- NASM

### 🛠️ Installation

1. Clone the repository:
    ```shell
    git clone https://github.com/Jlisowskyy/AlkOS
    ```

2. Navigate to the scripts directory:
    ```shell
    cd alkos/scripts
    ```

3. Run the CLI script to install dependencies and toolchain:
    ```shell
    ./alkos_cli.bash --install all --verbose
    ```

### 🏗️ Building the Project

1. Navigate to the scripts directory
    ```shell
    cd alkos/scripts
    ```
2. Run Alkos in QEMU
    ```shell
    ./alkos_cli.bash -v -r
     ```
