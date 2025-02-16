#include <extensions/tuple.hpp>
#include <extensions/type_traits.hpp>
#include <test_module/test.hpp>

class LibCxxTest : public TestGroupBase
{
};

TEST_F(LibCxxTest, IsSame)
{
    EXPECT_TRUE((std::is_same_v<int, int>));
    EXPECT_TRUE((std::is_same_v<double, double>));
    EXPECT_TRUE((std::is_same_v<u64, uint64_t>));

    EXPECT_FALSE((std::is_same_v<float, double>));
    EXPECT_FALSE((std::is_same_v<float, int>));
    EXPECT_FALSE((std::is_same_v<u64, i32>));
}

TEST_F(LibCxxTest, RemoveReference)
{
    EXPECT_TRUE((std::is_same_v<int, std::remove_reference_t<int>>));
    EXPECT_TRUE((std::is_same_v<int, std::remove_reference_t<int &>>));
    EXPECT_TRUE((std::is_same_v<int, std::remove_reference_t<int &&>>));

    EXPECT_TRUE((std::is_same_v<double, std::remove_reference_t<double>>));
    EXPECT_TRUE((std::is_same_v<double, std::remove_reference_t<double &>>));
    EXPECT_TRUE((std::is_same_v<double, std::remove_reference_t<double &&>>));
}

TEST_F(LibCxxTest, Identity)
{
    const auto func = []<class T>(T a, std::type_identity_t<T> b) {
        return a + b;
    };

    EXPECT_TRUE((std::is_same_v<int, std::type_identity_t<int>>));
    EXPECT_TRUE((std::is_same_v<double, std::type_identity_t<double>>));
    EXPECT_TRUE((std::is_same_v<u64, std::type_identity_t<u64>>));

    EXPECT_EQ(4, func(2, 2.5));
}

TEST_F(LibCxxTest, AddPointer)
{
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<int>, int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<const int>, const int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<int &>, int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<const int &>, const int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<int &&>, int *>));
}

TEST_F(LibCxxTest, Conditional)
{
    EXPECT_TRUE((std::is_same_v<std::conditional_t<false, double, float>, float>));
    EXPECT_TRUE((std::is_same_v<std::conditional_t<true, double, float>, double>));
}

TEST_F(LibCxxTest, RemovePointer)
{
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<const int *>, const int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *const>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *const volatile>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *volatile>, int>));
}

TEST_F(LibCxxTest, RemoveExtent)
{
    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<int>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<int[]>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<int[10]>, int>));

    int arr[]          = {1, 2, 3};
    const int arr1[]   = {1, 2, 3};
    const int arr2[10] = {1, 2, 3};

    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<decltype(arr)>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<decltype(arr1)>, const int>));
    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<decltype(arr2)>, const int>));

    int dim2arr[2][2]{
        {1, 2},
        {3, 4}
    };

    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<decltype(dim2arr)>, int[2]>));
    EXPECT_TRUE((std::is_same_v<std::remove_extent_t<std::remove_extent_t<decltype(dim2arr)>>, int>)
    );
}

TEST_F(LibCxxTest, IsConst)
{
    EXPECT_TRUE((std::is_const_v<const int>));
    EXPECT_FALSE((std::is_const_v<int>));
    EXPECT_FALSE((std::is_const_v<const int *>));
    EXPECT_FALSE((std::is_const_v<int *>));
    EXPECT_FALSE((std::is_const_v<const int &>));
    EXPECT_FALSE((std::is_const_v<int &>));
}

TEST_F(LibCxxTest, IsReference)
{
    EXPECT_TRUE((std::is_reference_v<int &>));
    EXPECT_TRUE((std::is_reference_v<int &&>));
    EXPECT_TRUE((std::is_reference_v<const int &>));
    EXPECT_FALSE((std::is_reference_v<const int>));
    EXPECT_FALSE((std::is_reference_v<int>));
}

static int f() { return 1; }

struct O {
    void operator()() {}
};

struct OO {
    static int foo() { return 2; }

    int fun() const &;
};

TEST_F(LibCxxTest, IsFunction)
{
    EXPECT_TRUE(std::is_function_v<decltype(f)>);
    EXPECT_TRUE(std::is_function_v<int(int)>);
    EXPECT_FALSE(std::is_function_v<int>);
    EXPECT_FALSE(std::is_function_v<decltype([] {
    })>);
    EXPECT_TRUE(std::is_function_v<O()>);
    EXPECT_FALSE(std::is_function_v<OO>);
    EXPECT_TRUE(std::is_function_v<decltype(OO::foo)>);
    EXPECT_FALSE(std::is_function_v<decltype(&OO::fun)>);
}

TEST_F(LibCxxTest, IsArray)
{
    EXPECT_FALSE((std::is_array_v<int>));
    EXPECT_TRUE((std::is_array_v<int[]>));
    EXPECT_TRUE((std::is_array_v<int[10]>));

    int arr[]          = {1, 2, 3};
    const int arr1[]   = {1, 2, 3};
    const int arr2[10] = {1, 2, 3};

    EXPECT_TRUE((std::is_array_v<decltype(arr)>));
    EXPECT_TRUE((std::is_array_v<decltype(arr1)>));
    EXPECT_TRUE((std::is_array_v<decltype(arr2)>));

    int dim2arr[2][2]{
        {1, 2},
        {3, 4}
    };

    EXPECT_TRUE((std::is_array_v<decltype(dim2arr)>));
    EXPECT_TRUE((std::is_array_v<std::remove_extent_t<decltype(dim2arr)>>));
}

TEST_F(LibCxxTest, Decay)
{
    EXPECT_TRUE((std::is_same_v<std::decay_t<int>, int>));
    EXPECT_FALSE((std::is_same_v<std::decay_t<int>, float>));
    EXPECT_TRUE((std::is_same_v<std::decay_t<int &>, int>));
    EXPECT_TRUE((std::is_same_v<std::decay_t<int &&>, int>));
    EXPECT_TRUE((std::is_same_v<std::decay_t<const int &>, int>));
    EXPECT_TRUE((std::is_same_v<std::decay_t<int[2]>, int *>));
    EXPECT_FALSE((std::is_same_v<std::decay_t<int[4][2]>, int *>));
    EXPECT_FALSE((std::is_same_v<std::decay_t<int[4][2]>, int **>));
    EXPECT_TRUE((std::is_same_v<std::decay_t<int[4][2]>, int(*)[2]>));
    EXPECT_TRUE((std::is_same_v<std::decay_t<int(int)>, int (*)(int)>));
    EXPECT_TRUE((std::is_same_v<std::decay_t<const char *const &>, const char *>));
}

TEST_F(LibCxxTest, IsLvalueReference)
{
    EXPECT_TRUE(std::is_lvalue_reference_v<int &>);
    EXPECT_TRUE(std::is_lvalue_reference_v<const int &>);
    EXPECT_FALSE(std::is_lvalue_reference_v<int &&>);
    EXPECT_FALSE(std::is_lvalue_reference_v<int>);
}

TEST_F(LibCxxTest, IsRvalueReference)
{
    EXPECT_FALSE(std::is_rvalue_reference_v<int &>);
    EXPECT_FALSE(std::is_rvalue_reference_v<const int &>);
    EXPECT_TRUE(std::is_rvalue_reference_v<int &&>);
    EXPECT_FALSE(std::is_rvalue_reference_v<int>);
}

TEST_F(LibCxxTest, TupleBasics)
{
    const auto tuple = std::make_tuple(1, 2, 3);
    EXPECT_EQ(1, std::get<0>(tuple));
    EXPECT_EQ(2, std::get<1>(tuple));
    EXPECT_EQ(3, std::get<2>(tuple));
    EXPECT_EQ(3_s, std::tuple_size<decltype(tuple)>::value);

    EXPECT_TRUE((std::is_same_v<int, std::tuple_element<0, decltype(tuple)>::type>));
    EXPECT_TRUE((std::is_same_v<int, std::tuple_element<1, decltype(tuple)>::type>));
    EXPECT_TRUE((std::is_same_v<int, std::tuple_element<2, decltype(tuple)>::type>));

    // const auto [a, b, c] = tuple;
    // EXPECT_EQ(1, a);
    // EXPECT_EQ(2, b);
    // EXPECT_EQ(3, c);
    //
    // EXPECT_TRUE((std::is_same_v<const int, decltype(a)>));
    // EXPECT_TRUE((std::is_same_v<const int, decltype(b)>));
    // EXPECT_TRUE((std::is_same_v<const int, decltype(c)>));

    // static constexpr const char *str = "abcd";
    // const auto tuple1 = std::make_tuple(static_cast<int>(1), 1.0, 2.0f, str);
    // EXPECT_EQ(static_cast<int>(1), tuple1.get<0>());
    // EXPECT_EQ(1.0, tuple1.get<1>());
    // EXPECT_EQ(2.0f, tuple1.get<2>());
    // EXPECT_STREQ(str, tuple1.get<3>());

    // EXPECT_TRUE((std::is_same_v<int, std::tuple_element<0, decltype(tuple1)>::type>));
    // EXPECT_TRUE((std::is_same_v<double, std::tuple_element<1, decltype(tuple1)>::type>));
    // EXPECT_TRUE((std::is_same_v<float, std::tuple_element<2, decltype(tuple1)>::type>));
    // EXPECT_TRUE((std::is_same_v<const char *, std::tuple_element<2, decltype(tuple1)>::type>));
    //
    // EXPECT_EQ(4_s, std::tuple_size_v<decltype(tuple1)>);

    // const auto [d, e, f, g] = tuple1;
    // EXPECT_EQ(static_cast<int>(1), d);
    // EXPECT_EQ(1.0, e);
    // EXPECT_EQ(2.0f, f);
    // EXPECT_STREQ(str, g);
    //
    // EXPECT_TRUE((std::is_same_v<const int, decltype(d)>));
    // EXPECT_TRUE((std::is_same_v<const double, decltype(e)>));
    // EXPECT_TRUE((std::is_same_v<const float, decltype(f)>));
    // EXPECT_TRUE((std::is_same_v<const char* const, decltype(g)>));
}

TEST_F(LibCxxTest, TupleGet) {

}

TEST_F(LibCxxTest, TupleElement) {

}

TEST_F(LibCxxTest, TupleBindings) {

}
