from typing import TextIO

from test_data import TestInfo
from test_log import TestLog
from test_utils import print_red, print_green


def _run_test(info: TestInfo, log_file: TextIO) -> bool:
    return False


def run_test(info: TestInfo, logger: TestLog) -> bool:
    with logger.save_log_with_context(info) as log_file:
        result = _run_test(info, log_file)

        if result:
            print_green(f"[PASS] Test: {info.test_name} correctly passed...")
        else:
            print_red(f"[FAIL] Test: {info.test_name} failed...")

        return result
