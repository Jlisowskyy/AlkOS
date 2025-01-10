#!/bin/python

import test_framework.main as tf


class TestRun:
    name: str
    test_filters: list[str]
    test_blockers: list[str]

    def __init__(self, name: str, test_filters: list[str], test_blockers: list[str]) -> None:
        self.name = name
        self.test_filters = test_filters
        self.test_blockers = test_blockers


TEST_RUNS: list[TestRun] = [
    TestRun(
        "Full test run",
        [],
        [
            "SnprintfTest_BufferSizeHandling",  # Fails <- issue added on low prio
            "SnprintfTest_EdgeCases",  # Fails <- issue added on low prio
        ]
    )
]


def prepare_command(test_run: TestRun) -> list[str]:
    args = []

    args.append("--path")
    args.append("./run_test_alkos.bash")

    if len(test_run.test_filters) > 0:
        args.append("--filter")

    for test_filter in test_run.test_filters:
        args.append(test_filter)

    args.append("--block")
    for test_blocker in test_run.test_blockers:
        args.append(test_blocker)

    return args


def run_command(args: list[str]) -> None:
    tf.main(args)


def main() -> None:
    for test_run in TEST_RUNS:
        print(f"[TEST] Running test run: {test_run.name}")

        command = prepare_command(test_run)
        run_command(command)


if __name__ == "__main__":
    main()
