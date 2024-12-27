from test_data import TestRunSpec, TestInfo
from test_parser import parse_tests
from test_runner import run_test
from test_log import TestLog
from test_utils import print_red, print_green


def _display_tests(tests: list[TestInfo]) -> None:
    print(f"Gathered {len(tests)} tests:")

    for test in tests:
        print(f"  {test.test_name}")


def test_framework_run(spec: TestRunSpec) -> None:
    file_logger = TestLog()

    tests_to_run = parse_tests(spec, file_logger)
    failed_tests = list[TestInfo]()

    # If specified display list of filtered tests and die
    if spec.display_tests_only:
        _display_tests(tests_to_run)
        return

    # Run the tests
    for test in tests_to_run:
        result = run_test(test, file_logger)

        if not result:
            failed_tests.append(test)

    if failed_tests:
        print_red(f"[FAILED] {len(failed_tests)} failed tests:")
        for test in failed_tests:
            print_red(f"  {test.test_name}")
        exit(1)
    else:
        print_green("[PASSED] All tests passed")
