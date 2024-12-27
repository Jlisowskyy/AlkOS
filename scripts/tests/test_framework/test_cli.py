import argparse
from pathlib import Path

from test_data import TestRunSpec


def parse_arguments(args: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description='Run tests on AlkOS kernel in controlled manner')

    parser.add_argument(
        '--path',
        type=str,
        required=True,
        help='Path to the file that will run the AlkOS kernel, built in test mode '
             '(in case of other builds behavior is undefined).'
    )

    parser.add_argument(
        '--filter',
        type=str,
        nargs='+',
        help='Run only filtered tests. Multiple patterns can be specified. '
             '\'*\' may be used as wildcard in each pattern.'
    )

    parser.add_argument(
        '--block',
        type=str,
        nargs='+',
        help='Exclude tests from running. Multiple patterns can be specified. '
             '\'*\' may be used as wildcard in each pattern.'
    )

    parser.add_argument(
        '--display',
        action='store_true',
        help='Display all tests that will be run according to flags specified.'
    )

    args = parser.parse_args(args)

    if not args.path:
        parser.error('The --path argument is required and cannot be empty')

    if not Path(args.path).exists():
        parser.error('The path specified does not exist')

    if not Path(args.path).is_file():
        parser.error('The path does not point to a file')

    return args


def process_args(args: list[str]) -> TestRunSpec:
    spec = TestRunSpec()

    raw_args = parse_arguments(args)

    spec.alkos_path = raw_args.path

    if raw_args.filter:
        spec.filters = [item.strip() for item in raw_args.filter]

    if raw_args.block:
        spec.blocks = [item.strip() for item in raw_args.block]

    return spec
