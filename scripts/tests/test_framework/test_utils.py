# ANSI color codes
RED = '\033[31m'
GREEN = '\033[32m'
RESET = '\033[0m'


def print_red(text: str) -> None:
    print(f"{RED}{text}{RESET}")


def print_green(text: str) -> None:
    print(f"{GREEN}{text}{RESET}")
