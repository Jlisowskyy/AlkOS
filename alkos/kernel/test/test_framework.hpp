#ifndef ALKOS_KERNEL_TEST_FRAMEWORK_HPP_
#define ALKOS_KERNEL_TEST_FRAMEWORK_HPP_

class TestFramework final
{
    using test_t = void (*)();

    // ------------------------------
    // Class creation
    // ------------------------------

    public:
    TestFramework()  = default;
    ~TestFramework() = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void RunTestModule();

    // ------------------------------
    // Class private methods
    // ------------------------------

    private:
    test_t FindTestFunction_(const char* name);
    void RunTest_(test_t test);

    // ------------------------------
    // Class fields
    // ------------------------------
};

#endif  // ALKOS_KERNEL_TEST_FRAMEWORK_HPP_
