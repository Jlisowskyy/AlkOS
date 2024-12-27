from test_data import TestRunSpec, TestInfo
from test_log import TestLog

import subprocess
import time
import select

MAX_ALKOS_TIME = 10


def _run_alkos_and_get_output(path: str, logger: TestLog) -> str:
    buf = ""

    try:
        alkos = subprocess.Popen(
            [path],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True,
            text=True,
            bufsize=1,
        )
        start_time = time.perf_counter_ns()

        # Read lines
        while True:
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

                print(f"Got line: {line}")
                buf += line

            if alkos.poll() is not None:
                stdout, stderr = alkos.communicate()

                if stdout:
                    buf += f"\n{stdout}\n"

                if stderr:
                    buf += f"\n{stderr}\n"

                break

    except Exception as e:
        print(f"[ERROR] Unexpected error running alkos: {e}")
        exit(1)
    finally:
        logger.save_init_log(buf)

    alkos.wait()
    return buf


def _parse_test_relevant_messages(output: str) -> list[str]:
    return []


def _parse_test_info(commands: list[str]) -> list[TestInfo]:
    return []


def _filter_tests(tests: list[TestInfo], filters: list[str], blocks: list[str]) -> list[TestInfo]:
    return tests


def parse_tests(spec: TestRunSpec, logger: TestLog) -> list[TestInfo]:
    output = _run_alkos_and_get_output(spec.alkos_path, logger)

    messages = _parse_test_relevant_messages(output)
    tests = _parse_test_info(messages)
    return _filter_tests(tests, spec.filters, spec.blocks)
