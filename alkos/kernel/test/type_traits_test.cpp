#include <test_module/test.hpp>
#include <extensions/type_traits.hpp>

class TypeTraitsTest : public TestGroupBase
{
};

TEST_F(TypeTraitsTest, IsSame)
{
    EXPECT_TRUE((std::is_same_v<int, int>));
    EXPECT_TRUE((std::is_same_v<double, double>));
    EXPECT_TRUE((std::is_same_v<u64, uint64_t>));

    EXPECT_FALSE((std::is_same_v<float, double>));
    EXPECT_FALSE((std::is_same_v<float, int>));
    EXPECT_FALSE((std::is_same_v<u64, i32>));
}

TEST_F(TypeTraitsTest, RemoveReference)
{
    EXPECT_TRUE((std::is_same_v<int, std::remove_reference_t<int>>));
    EXPECT_TRUE((std::is_same_v<int, std::remove_reference_t<int &>>));
    EXPECT_TRUE((std::is_same_v<int, std::remove_reference_t<int &&>>));

    EXPECT_TRUE((std::is_same_v<double, std::remove_reference_t<double>>));
    EXPECT_TRUE((std::is_same_v<double, std::remove_reference_t<double &>>));
    EXPECT_TRUE((std::is_same_v<double, std::remove_reference_t<double &&>>));
}

TEST_F(TypeTraitsTest, Identity)
{
    const auto func = []<class T>(T a, std::type_identity_t<T> b) {
        return a + b;
    };

    EXPECT_TRUE((std::is_same_v<int, std::type_identity_t<int>>));
    EXPECT_TRUE((std::is_same_v<double, std::type_identity_t<double>>));
    EXPECT_TRUE((std::is_same_v<u64, std::type_identity_t<u64>>));

    EXPECT_EQ(4, func(2, 2.5));
}

TEST_F(TypeTraitsTest, AddPointer)
{
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<int>, int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<const int>, const int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<int &>, int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<const int &>, const int *>));
    EXPECT_TRUE((std::is_same_v<std::add_pointer_t<int &&>, int *>));
}

TEST_F(TypeTraitsTest, Conditional)
{
    EXPECT_TRUE((std::is_same_v<std::conditional_t<false, double, float>, float>));
    EXPECT_TRUE((std::is_same_v<std::conditional_t<true, double, float>, double>));
}

TEST_F(TypeTraitsTest, RemovePointer)
{
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<const int *>, const int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *const>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *const volatile>, int>));
    EXPECT_TRUE((std::is_same_v<std::remove_pointer_t<int *volatile>, int>));
}

TEST_F(TypeTraitsTest, RemoveExtent)
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

TEST_F(TypeTraitsTest, IsConst)
{
    EXPECT_TRUE((std::is_const_v<const int>));
    EXPECT_FALSE((std::is_const_v<int>));
    EXPECT_FALSE((std::is_const_v<const int *>));
    EXPECT_FALSE((std::is_const_v<int *>));
    EXPECT_FALSE((std::is_const_v<const int &>));
    EXPECT_FALSE((std::is_const_v<int &>));
}

TEST_F(TypeTraitsTest, IsReference)
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

TEST_F(TypeTraitsTest, IsFunction)
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

TEST_F(TypeTraitsTest, IsArray)
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

TEST_F(TypeTraitsTest, Decay)
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

TEST_F(TypeTraitsTest, IsLvalueReference)
{
    EXPECT_TRUE(std::is_lvalue_reference_v<int &>);
    EXPECT_TRUE(std::is_lvalue_reference_v<const int &>);
    EXPECT_FALSE(std::is_lvalue_reference_v<int &&>);
    EXPECT_FALSE(std::is_lvalue_reference_v<int>);
}

TEST_F(TypeTraitsTest, IsRvalueReference)
{
    EXPECT_FALSE(std::is_rvalue_reference_v<int &>);
    EXPECT_FALSE(std::is_rvalue_reference_v<const int &>);
    EXPECT_TRUE(std::is_rvalue_reference_v<int &&>);
    EXPECT_FALSE(std::is_rvalue_reference_v<int>);
}

TEST_F(TypeTraitsTest, IsVoid) {
    EXPECT_TRUE(std::is_void_v<void>);
    EXPECT_TRUE(std::is_void_v<const void>);
    EXPECT_TRUE(std::is_void_v<const volatile void>);
    EXPECT_FALSE(std::is_void_v<int>);
    EXPECT_FALSE(std::is_void_v<const int>);
    EXPECT_FALSE(std::is_void_v<const volatile int>);
    EXPECT_FALSE(std::is_void_v<void*>);
    EXPECT_TRUE(std::is_void_v<volatile void>);
    EXPECT_FALSE(std::is_void_v<std::is_void<void>>);
}

TEST_F(TypeTraitsTest, IsNullPtr) {
    EXPECT_TRUE(std::is_null_pointer_v<std::nullptr_t>);
    EXPECT_TRUE(std::is_null_pointer_v<const std::nullptr_t>);
    EXPECT_TRUE(std::is_null_pointer_v<const volatile std::nullptr_t>);
    EXPECT_TRUE(std::is_null_pointer_v<decltype(nullptr)>);
    EXPECT_FALSE(std::is_null_pointer_v<int>);
    EXPECT_FALSE(std::is_null_pointer_v<const int>);
    EXPECT_FALSE(std::is_null_pointer_v<void*>);
}

TEST_F(TypeTraitsTest, IsIntegral) {
    EXPECT_TRUE(std::is_integral_v<int>);
    EXPECT_TRUE(std::is_integral_v<unsigned int>);
    EXPECT_TRUE(std::is_integral_v<short>);
    EXPECT_TRUE(std::is_integral_v<unsigned short>);
    EXPECT_TRUE(std::is_integral_v<long>);
    EXPECT_TRUE(std::is_integral_v<unsigned long>);
    EXPECT_TRUE(std::is_integral_v<long long>);
    EXPECT_TRUE(std::is_integral_v<unsigned long long>);
    EXPECT_TRUE(std::is_integral_v<char>);
    EXPECT_TRUE(std::is_integral_v<unsigned char>);
    EXPECT_TRUE(std::is_integral_v<signed char>);
    EXPECT_TRUE(std::is_integral_v<wchar_t>);
    EXPECT_TRUE(std::is_integral_v<char8_t>);
    EXPECT_TRUE(std::is_integral_v<char16_t>);
    EXPECT_TRUE(std::is_integral_v<char32_t>);

    EXPECT_FALSE(std::is_integral_v<float>);
    EXPECT_FALSE(std::is_integral_v<double>);
    EXPECT_FALSE(std::is_integral_v<long double>);
    EXPECT_FALSE(std::is_integral_v<void>);
    EXPECT_FALSE(std::is_integral_v<int*>);
    EXPECT_FALSE(std::is_integral_v<int&>);
    EXPECT_FALSE(std::is_integral_v<int[]>);
    EXPECT_FALSE(std::is_integral_v<int(int)>);
}

TEST_F(TypeTraitsTest, IsFloatingPoint) {
    EXPECT_TRUE(std::is_floating_point_v<float>);
    EXPECT_TRUE(std::is_floating_point_v<double>);
    EXPECT_TRUE(std::is_floating_point_v<long double>);

    EXPECT_FALSE(std::is_floating_point_v<int>);
    EXPECT_FALSE(std::is_floating_point_v<unsigned int>);
    EXPECT_FALSE(std::is_floating_point_v<short>);
    EXPECT_FALSE(std::is_floating_point_v<unsigned short>);
    EXPECT_FALSE(std::is_floating_point_v<long>);
    EXPECT_FALSE(std::is_floating_point_v<unsigned long>);
    EXPECT_FALSE(std::is_floating_point_v<long long>);
    EXPECT_FALSE(std::is_floating_point_v<unsigned long long>);
    EXPECT_FALSE(std::is_floating_point_v<void>);
    EXPECT_FALSE(std::is_floating_point_v<float*>);
    EXPECT_FALSE(std::is_floating_point_v<float&>);
    EXPECT_FALSE(std::is_floating_point_v<float[]>);
    EXPECT_FALSE(std::is_floating_point_v<float(float)>);
}

union SampleUnion {
    int a;
    float b;
};

enum SampleEnum {
    Value1,
    Value2,
    Value3
};

TEST_F(TypeTraitsTest, IsUnion) {
    EXPECT_TRUE(std::is_union_v<SampleUnion>);
    EXPECT_FALSE(std::is_union_v<int>);
    EXPECT_FALSE(std::is_union_v<float>);
    EXPECT_FALSE(std::is_union_v<SampleEnum>);
    EXPECT_FALSE(std::is_union_v<void>);
}

TEST_F(TypeTraitsTest, IsEnum) {
    EXPECT_TRUE(std::is_enum_v<SampleEnum>);
    EXPECT_FALSE(std::is_enum_v<int>);
    EXPECT_FALSE(std::is_enum_v<float>);
    EXPECT_FALSE(std::is_enum_v<SampleUnion>);
    EXPECT_FALSE(std::is_enum_v<void>);
}

TEST_F(TypeTraitsTest, IsPointer) {
    EXPECT_TRUE(std::is_pointer_v<int *>);
    EXPECT_TRUE(std::is_pointer_v<const int *>);
    EXPECT_TRUE(std::is_pointer_v<int *const>);
    EXPECT_TRUE(std::is_pointer_v<int *volatile>);
    EXPECT_TRUE(std::is_pointer_v<int *const volatile>);

    EXPECT_FALSE(std::is_pointer_v<int>);
    EXPECT_FALSE(std::is_pointer_v<const int>);
    EXPECT_FALSE(std::is_pointer_v<int &>);
    EXPECT_FALSE(std::is_pointer_v<int &&>);
    EXPECT_FALSE(std::is_pointer_v<int[]>);
    EXPECT_FALSE(std::is_pointer_v<int[10]>);
    EXPECT_FALSE(std::is_pointer_v<void>);
    EXPECT_FALSE(std::is_pointer_v<std::nullptr_t>);
}

TEST_F(TypeTraitsTest, IsClass) {
    struct A {};
    class B {};
    union U { int a; };
    enum E { Value1, Value2 };
    enum class C {};

    auto lambda = [](){};

    EXPECT_TRUE((std::is_class_v<A>));
    EXPECT_TRUE((std::is_class_v<B>));

    EXPECT_TRUE((std::is_class_v<decltype(lambda)>));

    EXPECT_FALSE((std::is_class_v<U>));

    EXPECT_FALSE((std::is_class_v<E>));
    EXPECT_FALSE((std::is_class_v<C>));

    EXPECT_FALSE((std::is_class_v<int>));
    EXPECT_FALSE((std::is_class_v<double>));
    EXPECT_FALSE((std::is_class_v<int*>));
    EXPECT_FALSE((std::is_class_v<int&>));
}

struct TestClass {
    int member;
    void func() {}
};

TEST_F(TypeTraitsTest, IsFundamental) {
    EXPECT_TRUE(std::is_fundamental_v<int>);
    EXPECT_TRUE(std::is_fundamental_v<float>);
    EXPECT_TRUE(std::is_fundamental_v<double>);
    EXPECT_TRUE(std::is_fundamental_v<char>);
    EXPECT_TRUE(std::is_fundamental_v<bool>);
    EXPECT_TRUE(std::is_fundamental_v<void>);
    EXPECT_TRUE(std::is_fundamental_v<std::nullptr_t>);

    EXPECT_FALSE(std::is_fundamental_v<int*>);
    EXPECT_FALSE(std::is_fundamental_v<int&>);
    EXPECT_FALSE(std::is_fundamental_v<int[]>);
    EXPECT_FALSE(std::is_fundamental_v<int(int)>);
    EXPECT_FALSE(std::is_fundamental_v<TestClass>);
}

TEST_F(TypeTraitsTest, IsMemberPointer) {
    EXPECT_TRUE(std::is_member_pointer_v<int TestClass::*>);
    EXPECT_TRUE(std::is_member_pointer_v<void (TestClass::*)()>);

    EXPECT_FALSE(std::is_member_pointer_v<int*>);
    EXPECT_FALSE(std::is_member_pointer_v<int>);
    EXPECT_FALSE(std::is_member_pointer_v<void()>);
}

TEST_F(TypeTraitsTest, IsScalar) {
    EXPECT_TRUE(std::is_scalar_v<int>);
    EXPECT_TRUE(std::is_scalar_v<float>);
    EXPECT_TRUE(std::is_scalar_v<double>);
    EXPECT_TRUE(std::is_scalar_v<char>);
    EXPECT_TRUE(std::is_scalar_v<bool>);
    EXPECT_TRUE(std::is_scalar_v<std::nullptr_t>);
    EXPECT_TRUE(std::is_scalar_v<int*>);
    EXPECT_TRUE(std::is_scalar_v<int TestClass::*>);
    EXPECT_TRUE(std::is_scalar_v<void (TestClass::*)()>);

    EXPECT_FALSE(std::is_scalar_v<void>);
    EXPECT_FALSE(std::is_scalar_v<int[]>);
    EXPECT_FALSE(std::is_scalar_v<int(int)>);
    EXPECT_FALSE(std::is_scalar_v<TestClass>);
}

TEST_F(TypeTraitsTest, IsObject) {
    struct TestClass {};
    union TestUnion {
        int a;
        float b;
    };

    EXPECT_TRUE(std::is_object_v<int>);
    EXPECT_TRUE(std::is_object_v<float>);
    EXPECT_TRUE(std::is_object_v<TestClass>);
    EXPECT_TRUE(std::is_object_v<TestUnion>);
    EXPECT_TRUE(std::is_object_v<int*>);
    EXPECT_TRUE(std::is_object_v<int[10]>);

    EXPECT_FALSE(std::is_object_v<void>);
    EXPECT_FALSE(std::is_object_v<int&>);
    EXPECT_FALSE(std::is_object_v<int(int)>);
}

TEST_F(TypeTraitsTest, IsSigned) {
    EXPECT_TRUE(std::is_signed_v<int>);
    EXPECT_TRUE(std::is_signed_v<short>);
    EXPECT_TRUE(std::is_signed_v<long>);
    EXPECT_TRUE(std::is_signed_v<long long>);
    EXPECT_TRUE(std::is_signed_v<float>);
    EXPECT_TRUE(std::is_signed_v<double>);
    EXPECT_TRUE(std::is_signed_v<long double>);

    EXPECT_FALSE(std::is_signed_v<unsigned int>);
    EXPECT_FALSE(std::is_signed_v<unsigned short>);
    EXPECT_FALSE(std::is_signed_v<unsigned long>);
    EXPECT_FALSE(std::is_signed_v<unsigned long long>);
    EXPECT_FALSE(std::is_signed_v<bool>);
    EXPECT_FALSE(std::is_signed_v<char>);
}

TEST_F(TypeTraitsTest, IsUnsigned) {
    EXPECT_TRUE(std::is_unsigned_v<unsigned int>);
    EXPECT_TRUE(std::is_unsigned_v<unsigned short>);
    EXPECT_TRUE(std::is_unsigned_v<unsigned long>);
    EXPECT_TRUE(std::is_unsigned_v<unsigned long long>);
    EXPECT_TRUE(std::is_unsigned_v<bool>);
    EXPECT_TRUE(std::is_unsigned_v<unsigned char>);

    EXPECT_FALSE(std::is_unsigned_v<int>);
    EXPECT_FALSE(std::is_unsigned_v<short>);
    EXPECT_FALSE(std::is_unsigned_v<long>);
    EXPECT_FALSE(std::is_unsigned_v<long long>);
    EXPECT_FALSE(std::is_unsigned_v<float>);
    EXPECT_FALSE(std::is_unsigned_v<double>);
    EXPECT_FALSE(std::is_unsigned_v<long double>);
}

TEST_F(TypeTraitsTest, IsAbstract) {
    struct AbstractBase { virtual void func() = 0; };
    struct Derived : AbstractBase { void func() override {} };

    EXPECT_TRUE(std::is_abstract_v<AbstractBase>);
    EXPECT_FALSE(std::is_abstract_v<Derived>);
}

TEST_F(TypeTraitsTest, IsPolymorphic) {
    struct Base { virtual void func() {} };
    struct Derived : Base {};
    struct NonVirtual {};

    EXPECT_TRUE(std::is_polymorphic_v<Base>);
    EXPECT_TRUE(std::is_polymorphic_v<Derived>);
    EXPECT_FALSE(std::is_polymorphic_v<NonVirtual>);
}

TEST_F(TypeTraitsTest, IsFinal) {
    struct Regular {};
    struct FinalClass final {};

    EXPECT_FALSE(std::is_final_v<Regular>);
    EXPECT_TRUE(std::is_final_v<FinalClass>);
}
