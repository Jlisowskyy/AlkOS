from test_data import TestRunSpec, TestInfo
from test_parser import parse_tests
from test_runner import run_test


def test_framework_run(spec: TestRunSpec) -> None:
    tests_to_run = parse_tests(spec)
    failed_tests = list[TestInfo]()

    for test in tests_to_run:
        result = run_test(test, "")

        if not result:
            failed_tests.append(test)

    if failed_tests:
        pass # display fail message