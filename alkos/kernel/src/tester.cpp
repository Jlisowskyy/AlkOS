/* main include */
#include <tester.hpp>

/* external include */
#include <stddef.h>

/**
 * @brief Test should drop kernel panic due to stack smashing
 */
static void StackSmashTest() {
    static constexpr uint64_t kStackSize = 32;
    static constexpr uint64_t kWriteSize = 64;

    char buff[kStackSize];

    for (size_t i = 0; i < kWriteSize; i++) {
        buff[i] = 'A';
    }
}

/**
 * @brief Test should not drop kernel panic due to enabled float extension
 */
static void FloatExtensionTest() {
    /* simple float test */
    float f1 = 0.1f;
    float f2 = 0.2f;

    f1 = f1 + f2;

    /* simple double test */
    double d1 = 0.1;
    double d2 = 0.2;

    d2 = d1 + d2;

    /* simple long double test */
    long double ld1 = 0.1;
    long double ld2 = 0.2;

    ld2 = ld1 + ld2;

    /* final sum */
    long double sum = f1 + d2 + ld2;
}

using TestFuncType = void (*)();
static TestFuncType TestTable[]{
    StackSmashTest,
    FloatExtensionTest,
};

static constexpr uint64_t kTestTableSize = sizeof(TestTable) == 0 ? 0 : sizeof(TestTable) / sizeof(TestTable[0]);
static_assert(kTestTableSize == TestType::kLastTest, "TestTable does not contain all tests");

void RunTest(const TestType type) {
    TestTable[static_cast<uint64_t>(type)]();
}
