/* main include */
#include <tester.hpp>

/* external include */
#include <kernel_assert.hpp>
#include <new.hpp>
#include <panic.hpp>
#include <terminal.hpp>

// ---------------------------------------
// cpp compatibility test components
// ---------------------------------------

class TestClass
{
    public:
    explicit TestClass(const int x) : m_a(x), m_b(2 * x + 2) {}

    int m_a;
    const int m_b;
};

TestClass g_GlobalTestClass(5);

static void CppTest()
{
    char mem[sizeof(TestClass)];

    /* Test static initializer */
    static TestClass test(5);
    test.m_a++;

    /* test usual new and delete */
    const auto *test2 = new int;
    delete test2;

    /* test array new and delete */
    const auto *test3 = new int[5];
    delete[] test3;

    /* test placement new and delete */
    auto *test4 = new (reinterpret_cast<TestClass *>(mem)) TestClass(5);
    test4->m_a++;

    /* test global object */
    ASSERT_EQ(12, g_GlobalTestClass.m_b);
}

// ------------------------------
// Stack Smash Test
// ------------------------------

/**
 * @brief Test should drop kernel panic due to stack smashing
 */
static void StackSmashTest()
{
    static constexpr uint64_t kStackSize = 32;
    static constexpr uint64_t kWriteSize = 64;

    char buff[kStackSize];

    for (size_t i = 0; i < kWriteSize; i++)
    {
        buff[i] = 'A';
    }
}

// ------------------------------
// Float operations test
// ------------------------------

/**
 * @brief Test should not drop kernel panic due to enabled float extension
 *
 * @note This test is architecture dependent, simply invokes floating point instructions
 */
extern void FloatExtensionTest();

// ------------------------------
// Simple Exception test
// ------------------------------

/**
 *  @brief Test should simply drop 0 division exception
 *
 */
static void ExceptionTest()
{
    int a = 9 / 0;
    TerminalWriteString("Exception test failed\n");
}

// ------------------------------
// Test table
// ------------------------------

using TestFuncType = void (*)();
static TestFuncType TestTable[]{
    StackSmashTest,
    CppTest,
    FloatExtensionTest,
    ExceptionTest,
};

static constexpr uint64_t kTestTableSize = sizeof(TestTable) == 0 ? 0 : sizeof(TestTable) / sizeof(TestTable[0]);
static_assert(kTestTableSize == kLastTest, "TestTable does not contain all tests");

void RunTest(const TestType type) { TestTable[static_cast<uint64_t>(type)](); }
