class TestRunSpec:
    alkos_path: str
    filters: list[str]
    blocks: list[str]
    display_tests_only: bool

    def __init__(self) -> None:
        self.alkos_path = ""
        self.filters = []
        self.blocks = []
        self.display_tests_only = False

class TestInfo:
    test_name: str
