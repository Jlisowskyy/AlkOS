from test_data import TestRunSpec, TestInfo


def _run_alkos_and_get_output(path: str) -> str:
    return ""


def _parse_test_relevant_messages(output: str) -> list[str]:
    return []


def _parse_test_info(commands: list[str]) -> list[TestInfo]:
    return []


def _filter_tests(tests: list[TestInfo], filters: list[str], blocks: list[str]) -> list[TestInfo]:
    return tests


def parse_tests(spec: TestRunSpec) -> list[TestInfo]:
    output = _run_alkos_and_get_output(spec.alkos_path)
    messages = _parse_test_relevant_messages(output)
    tests = _parse_test_info(messages)
    return _filter_tests(tests, spec.filters, spec.blocks)
