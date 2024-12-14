/* main include */
#include "../test/tester.hpp"

/* external include */
#include <stddef.h>

static void StackSmashTest() {
    static constexpr uint64_t kStackSize = 32;
    static constexpr uint64_t kWriteSize = 64;

    char buff[kStackSize];

    for (size_t i = 0; i < kWriteSize; i++) {
        buff[i] = 'A';
    }
}

using TestFuncType = void (*)();
static TestFuncType TestTable[]{
    StackSmashTest,
};

static constexpr uint64_t kTestTableSize = sizeof(TestTable) == 0 ? 0 : sizeof(TestTable) / sizeof(TestTable[0]);
static_assert(kTestTableSize == TestType::kLastTest, "TestTable does not contain all tests");

void RunTest(const TestType type) {
    TestTable[static_cast<uint64_t>(type)]();
}
