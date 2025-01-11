#include <test_module/test.hpp>

// ------------------------------
// R_ASSERT_EQ Tests
// ------------------------------

TEST(RAssertEqPass)
{
    R_ASSERT_EQ(5, 5);
    R_ASSERT_EQ(true, true);
    R_ASSERT_EQ(nullptr, nullptr);
}

FAIL_TEST(RAssertEqFail) { R_ASSERT_EQ(5, 10); }

// ------------------------------
// R_ASSERT_NEQ Tests
// ------------------------------

TEST(RAssertNeqPass)
{
    R_ASSERT_NEQ(5, 10);
    R_ASSERT_NEQ(true, false);
    int value = 5;
    R_ASSERT_NEQ(&value, nullptr);
}

FAIL_TEST(RAssertNeqFail) { R_ASSERT_NEQ(5, 5); }

// ------------------------------
// R_ASSERT_TRUE Tests
// ------------------------------

TEST(RAssertTruePass)
{
    R_ASSERT_TRUE(true);
    R_ASSERT_TRUE(1 == 1);
    R_ASSERT_TRUE(5 > 3);
}

FAIL_TEST(RAssertTrueFail) { R_ASSERT_TRUE(false); }

// ------------------------------
// R_ASSERT_FALSE Tests
// ------------------------------

TEST(RAssertFalsePass)
{
    R_ASSERT_FALSE(false);
    R_ASSERT_FALSE(1 == 2);
    R_ASSERT_FALSE(5 < 3);
}

FAIL_TEST(RAssertFalseFail) { R_ASSERT_FALSE(true); }

// ------------------------------
// R_ASSERT_NOT_NULL Tests
// ------------------------------

TEST(RAssertNotNullPass)
{
    int value = 5;
    int *ptr  = &value;
    R_ASSERT_NOT_NULL(ptr);
}

FAIL_TEST(RAssertNotNullFail)
{
    int *ptr = nullptr;
    R_ASSERT_NOT_NULL(ptr);
}

// ------------------------------
// R_ASSERT_NULL Tests
// ------------------------------

TEST(RAssertNullPass)
{
    int *ptr = nullptr;
    R_ASSERT_NULL(ptr);
}

FAIL_TEST(RAssertNullFail)
{
    int value = 5;
    int *ptr  = &value;
    R_ASSERT_NULL(ptr);
}

// ------------------------------
// R_ASSERT_LT Tests
// ------------------------------

TEST(RAssertLtPass) { R_ASSERT_LT(5, 10); }

FAIL_TEST(RAssertLtFail) { R_ASSERT_LT(10, 5); }

// ------------------------------
// R_ASSERT_LE Tests
// ------------------------------

TEST(RAssertLePass)
{
    R_ASSERT_LE(5, 5);
    R_ASSERT_LE(5, 10);
}

FAIL_TEST(RAssertLeFail) { R_ASSERT_LE(10, 5); }

// ------------------------------
// R_ASSERT_GT Tests
// ------------------------------

TEST(RAssertGtPass) { R_ASSERT_GT(10, 5); }

FAIL_TEST(RAssertGtFail) { R_ASSERT_GT(5, 10); }

// ------------------------------
// R_ASSERT_GE Tests
// ------------------------------

TEST(RAssertGePass)
{
    R_ASSERT_GE(5, 5);
    R_ASSERT_GE(10, 5);
}

FAIL_TEST(RAssertGeFail) { R_ASSERT_GE(5, 10); }

// ------------------------------
// R_ASSERT_STREQ Tests
// ------------------------------

TEST(RAssertStrEqPass)
{
    const char *str1 = "test";
    const char *str2 = "test";
    R_ASSERT_STREQ(str1, str2);
}

FAIL_TEST(RAssertStrEqFail)
{
    const char *str1 = "test1";
    const char *str2 = "test2";
    R_ASSERT_STREQ(str1, str2);
}

// ------------------------------
// R_ASSERT_STRNEQ Tests
// ------------------------------

TEST(RAssertStrNeqPass)
{
    const char *str1 = "test1";
    const char *str2 = "test2";
    R_ASSERT_STRNEQ(str1, str2);
}

FAIL_TEST(RAssertStrNeqFail)
{
    const char *str1 = "test";
    const char *str2 = "test";
    R_ASSERT_STRNEQ(str1, str2);
}
