import select
import subprocess
import time
from typing import TextIO

from test_commands import TEST_COMMAND_SPECIFIER, TEST_SUCCESS_COMMAND, TEST_FAIL_COMMAND
from test_data import MAX_ALKOS_TEST_TIME, MAX_ALKOS_WAIT_SYNC_TIME, TestState
from test_data import TestInfo
from test_log import TestLog
from test_utils import print_red, print_green


def _process_test_state(test_state: TestState, line: str) -> TestState:
    assert TEST_COMMAND_SPECIFIER in line

    if test_state == TestState.UNKNOWN:
        if TEST_SUCCESS_COMMAND in line:
            return TestState.SUCCESS
        elif TEST_FAIL_COMMAND in line:
            return TestState.FAILURE

    elif test_state == TestState.SUCCESS:
        if TEST_SUCCESS_COMMAND in line:
            return TestState.ERROR_DOUBLE_SUCCESS
        if TEST_FAIL_COMMAND in line:
            return TestState.ERROR_FAIL_AFTER_SUCCESS

    elif test_state == TestState.FAILURE:
        if TEST_FAIL_COMMAND in line:
            return TestState.ERROR_FAIL_AFTER_FAIL
        if TEST_SUCCESS_COMMAND in line:
            return TestState.ERROR_SUCCESS_AFTER_FAIL

    return test_state


def _run_test(path: str, info: TestInfo, log_file: TextIO) -> bool:
    state = TestState.UNKNOWN

    try:
        alkos = subprocess.Popen(
            [path],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        start_time = time.perf_counter_ns()

        # Write test name on input and process the output
        alkos.stdin.write(info.test_name + '\r')
        alkos.stdin.flush()

        # Read lines
        while True:
            reads = [alkos.stdout, alkos.stderr]

            curr_time = time.perf_counter_ns()
            readable, _, _ = select.select(reads, [], [], MAX_ALKOS_TEST_TIME - (curr_time - start_time) * 1e-9)

            if not readable:
                alkos.kill()
                alkos.wait()

                stdout, stderr = alkos.communicate()

                if stdout:
                    log_file.write(stdout)

                if stderr:
                    log_file.write(stderr)

                raise Exception("Readable failed -> timed out or stream failed")

            for stream in readable:
                line = stream.readline()

                if not line:
                    break

                if TEST_COMMAND_SPECIFIER in line:
                    state = _process_test_state(state, line)

                log_file.write(line)

            if alkos.poll() is not None:
                break

    except Exception as e:
        print(f"[ERROR] Unexpected error running alkos: {e}")
        exit(1)

    try:
        alkos.wait(MAX_ALKOS_WAIT_SYNC_TIME)
        stdout, stderr = alkos.communicate()

        if stdout:
            log_file.write(stdout)

        if stderr:
            log_file.write(stderr)

    except Exception as e:
        print(f"[ERROR] Unexpected error waiting for alkos: {e}")
        exit(1)

    if state != TestState.SUCCESS and state != TestState.FAILURE:
        print(f"[WARNING] Test failed with unexpected error: {str(state)}", file=stderr)

    return state == TestState.SUCCESS

def run_test(path: str, info: TestInfo, logger: TestLog) -> bool:
    with logger.save_log_with_context(info) as log_file:
        result = _run_test(path, info, log_file)

        if result:
            print_green(f"[PASS] Test: {info.test_name} correctly passed...")
        else:
            print_red(f"[FAIL] Test: {info.test_name} failed...")

        return result
