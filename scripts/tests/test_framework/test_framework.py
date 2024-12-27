from test_data import TestRunSpec, TestInfo
from test_parser import parse_tests
from test_runner import run_test
from test_log import TestLog

def test_framework_run(spec: TestRunSpec) -> None:
    file_logger = TestLog()

    tests_to_run = parse_tests(spec, file_logger)
    failed_tests = list[TestInfo]()

    for test in tests_to_run:
        result = run_test(test, file_logger)

        if not result:
            failed_tests.append(test)

    if failed_tests:
        pass # display fail message