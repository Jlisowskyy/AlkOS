#include <extensions/tuple.hpp>
#include <extensions/type_traits.hpp>
#include <test_module/test.hpp>

class TestClass
{
    public:
    explicit TestClass(const int x) : m_a(x), m_b(2 * x + 2) {}

    int m_a;
    const int m_b;
};

TestClass g_GlobalTestClass(5);

/**
 * @brief no exception should be thrown and global object should be correctly initialized
 */
TEST(CppTest)
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
    R_ASSERT_EQ(12, g_GlobalTestClass.m_b);
}

class AlignedTestClass : public TestGroupBase
{
    public:
    alignas(256) char m_mem[256];
};

TEST_F(AlignedTestClass, TestAlignas)
{
    R_ASSERT_EQ(static_cast<size_t>(0), reinterpret_cast<size_t>(m_mem) % 256);
}

TEST(IsSameTest)
{
    EXPECT_TRUE((std::is_same_v<int, int>));
    EXPECT_TRUE((std::is_same_v<double, double>));
    EXPECT_TRUE((std::is_same_v<u64, uint64_t>));

    EXPECT_FALSE((std::is_same_v<float, double>));
    EXPECT_FALSE((std::is_same_v<float, int>));
    EXPECT_FALSE((std::is_same_v<u64, i32>));
}

TEST(TupleTest)
{
    const auto tuple = std::make_tuple(1, 2, 3);
    EXPECT_EQ(1, tuple.get<0>());
    EXPECT_EQ(2, tuple.get<1>());
    EXPECT_EQ(3, tuple.get<2>());
    EXPECT_EQ(static_cast<size_t>(3), std::tuple_size<decltype(tuple)>::value);

    const auto [a, b, c] = tuple;
    EXPECT_EQ(1, a);
    EXPECT_EQ(2, b);
    EXPECT_EQ(3, c);

    EXPECT_TRUE((std::is_same_v<int, std::tuple_element<0, decltype(tuple)>::type>));
    EXPECT_TRUE((std::is_same_v<int, std::tuple_element<1, decltype(tuple)>::type>));
    EXPECT_TRUE((std::is_same_v<int, std::tuple_element<2, decltype(tuple)>::type>));

    static constexpr const char *str = "abcd";
    const auto tuple1                = std::make_tuple(static_cast<int>(1), 1.0, 2.0f, str);
    EXPECT_EQ(static_cast<int>(1), tuple1.get<0>());
    EXPECT_EQ(1.0, tuple1.get<1>());
    EXPECT_EQ(2.0f, tuple1.get<2>());
    EXPECT_STREQ(str, tuple1.get<3>());

    EXPECT_TRUE((std::is_same_v<int, std::tuple_element<0, decltype(tuple1)>::type>));
    EXPECT_TRUE((std::is_same_v<double, std::tuple_element<1, decltype(tuple1)>::type>));
    EXPECT_TRUE((std::is_same_v<float, std::tuple_element<2, decltype(tuple1)>::type>));

    /* TODO: reference? */
    EXPECT_TRUE((std::is_same_v<const char *const &, std::tuple_element<2, decltype(tuple1)>::type>)
    );

    EXPECT_EQ(static_cast<size_t>(4), std::tuple_size_v<decltype(tuple1)>);

    const auto [d, e, f, g] = tuple1;
    EXPECT_EQ(static_cast<int>(1), d);
    EXPECT_EQ(1.0, e);
    EXPECT_EQ(2.0f, f);
    EXPECT_STREQ(str, g);
}
