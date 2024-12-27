from test_data import TestRunSpec, TestInfo
from test_log import TestLog
from test_commands import TEST_DISPLAY_STOP_COMMAND_IN, TEST_DISPLAY_STOP_COMMAND_OUT, TEST_COMMAND_SPECIFIER, \
    TEST_NAME_SPECIFIER

import subprocess
import time
import select
import fnmatch

MAX_ALKOS_TIME = 10
MAX_ALKOS_WAIT_TIME = 10


def _run_alkos_and_get_output(path: str, logger: TestLog) -> str:
    buf = ""
    should_process = True

    try:
        alkos = subprocess.Popen(
            [path],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        start_time = time.perf_counter_ns()

        # Read lines
        while should_process:
            reads = [alkos.stdout, alkos.stderr]

            curr_time = time.perf_counter_ns()
            readable, _, _ = select.select(reads, [], [], MAX_ALKOS_TIME - (curr_time - start_time) * 1e-9)

            if not readable:
                alkos.kill()
                alkos.wait()

                stdout, stderr = alkos.communicate()

                if stdout:
                    buf += f"\n{stdout}\n"

                if stderr:
                    buf += f"\n{stderr}\n"

                raise Exception("Readable failed -> timed out or stream failed")

            for stream in readable:
                line = stream.readline()

                if not line:
                    raise Exception("Read failed...")

                buf += line

                if line == TEST_DISPLAY_STOP_COMMAND_IN:
                    alkos.stdin.write(TEST_DISPLAY_STOP_COMMAND_OUT)
                    alkos.stdin.flush()
                    should_process = False

            if alkos.poll() is not None:
                break

    except Exception as e:
        print(f"[ERROR] Unexpected error running alkos: {e}")
        logger.save_init_log(buf)
        exit(1)

    try:
        alkos.wait(MAX_ALKOS_WAIT_TIME)
        stdout, stderr = alkos.communicate()

        if stdout:
            buf += f"\n{stdout}\n"

        if stderr:
            buf += f"\n{stderr}\n"

    except Exception as e:
        print(f"[ERROR] Unexpected error waiting for alkos: {e}")
        exit(1)
    finally:
        logger.save_init_log(buf)

    return buf


def _parse_test_relevant_messages(output: str) -> list[str]:
    lines = output.splitlines()
    commands = list[str]()

    for line in lines:
        split = line.split()

        if len(split) == 0:
            continue

        if split[0] == TEST_COMMAND_SPECIFIER:
            commands.append(' '.join(split[1:]))

    return commands


def _parse_test_info(commands: list[str]) -> list[TestInfo]:
    test_infos = list[TestInfo]()

    for command in commands:
        split = command.split()

        if len(split) == 0:
            continue

        if split[0] == TEST_NAME_SPECIFIER:
            if len(split) != 2:
                raise Exception(f"Test name is expected to be single non blank separated sequence of characters."
                                f"Got invalid test name: {' '.join(split[1:])}")

            info = TestInfo()
            info.test_name = split[1]
            test_infos.append(info)

    return test_infos


def _filter_tests(tests: list[TestInfo], filters: list[str], blocks: list[str]) -> list[TestInfo]:
    filtered_tests = []

    for test in tests:
        is_blocked = False

        for block in blocks:
            if fnmatch.fnmatch(test.test_name, block):
                is_blocked = True
                break

        if is_blocked:
            continue

        is_filtered = False

        for test_filter in filters:
            if not fnmatch.fnmatch(test.test_name, test_filter):
                is_filtered = True
                break

        if is_filtered:
            continue

        filtered_tests.append(test)

    return filtered_tests


def parse_tests(spec: TestRunSpec, logger: TestLog) -> list[TestInfo]:
    output = _run_alkos_and_get_output(spec.alkos_path, logger)

    messages = _parse_test_relevant_messages(output)

    tests = _parse_test_info(messages)

    return _filter_tests(tests, spec.filters, spec.blocks)
