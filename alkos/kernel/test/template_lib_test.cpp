#include <test_module/test.hpp>
#include <extensions/template_lib.hpp>

class TemplateLibTest : public TestGroupBase
{
};

struct TestFunc {
    template<uint64_t N>
    void operator()(int &out) const {
        out = static_cast<int>(N);
    }
};

struct TestFuncReturn {
    template<uint64_t N>
    int operator()() const {
        return static_cast<int>(N);
    }
};

void DefaultFunc(int &out) {
    out = -1;
}

int DefaultFuncReturn() {
    return -1;
}

TEST_F(TemplateLibTest, RolledSwitchTest) {
    int result = -1;
    constexpr uint64_t kMaxValue = 10;
    constexpr uint64_t kStep = 2;

    // Test case where value matches max value
    RolledSwitch<int, kMaxValue, kStep>(DefaultFunc, TestFunc{}, 10, result);
    EXPECT_EQ(result, 10);

    // Test case where value is within range but not max
    RolledSwitch<int, kMaxValue, kStep>(DefaultFunc, TestFunc{}, 8, result);
    EXPECT_EQ(result, 8);

    // Test case where value does not match any case (fallback)
    RolledSwitch<int, kMaxValue, kStep>(DefaultFunc, TestFunc{}, 7, result);
    EXPECT_EQ(result, -1);

    // Test case where value is bigger than kMaxValue
    RolledSwitch<int, kMaxValue, kStep>(DefaultFunc, TestFunc{}, 11, result);
    EXPECT_EQ(result, -1);
}

TEST_F(TemplateLibTest, RolledSwitchReturnable) {
    constexpr uint64_t kMaxValue = 10;
    constexpr uint64_t kStep = 2;

    // Test case for RolledSwitchReturnable with a matching value
    int returnResult = RolledSwitchReturnable<int, kMaxValue, kStep>(DefaultFuncReturn, TestFuncReturn{}, 10);
    EXPECT_EQ(returnResult, 10);

    // Test case for RolledSwitchReturnable with a non-matching value (fallback)
    returnResult = RolledSwitchReturnable<int, kMaxValue, kStep>(DefaultFuncReturn, TestFuncReturn{}, 7);
    EXPECT_EQ(returnResult, -1);

    // Test case for RolledSwitchReturnable with a value bigger than kMaxValue
    returnResult = RolledSwitchReturnable<int, kMaxValue, kStep>(DefaultFuncReturn, TestFuncReturn{}, 11);
    EXPECT_EQ(returnResult, -1);
}