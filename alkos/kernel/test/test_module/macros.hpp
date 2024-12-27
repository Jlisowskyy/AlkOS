#ifndef ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_
#define ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_

#include <test_module/test_group_base.hpp>
#include <test_module/test_module.hpp>

// ------------------------------
// Base macros
// ------------------------------

using namespace test;

#define ___TEST_TEST_CLASS_NAME(fixture, test_name) \
    ___test_test_obj_##fixture##_##test_name##_743524762436

#define ___TEST_TEST_FACTORY_NAME(fixture, test_name) \
    ___TEST_TEST_CLASS_NAME(fixture, test_name) _factory

#define ___TEST_TEST_NAME(fixture, test_name) fixture##.##test_name

#define ___TEST_F(fixture, test, expect_fail)                                                     \
    class ___TEST_TEST_CLASS_NAME(fixture, test_name) final : public fixture                      \
    {                                                                                             \
        public:                                                                                   \
        ___TEST_TEST_CLASS_NAME(test_name)()          = default;                                  \
        virtual ~___TEST_TEST_CLASS_NAME(test_name)() = default;                                  \
        void SetExpectFail_() { g_expectFail = expect_fail; }                                     \
        void Run_() override;                                                                     \
                                                                                                  \
        private:                                                                                  \
        static const bool registered_;                                                            \
    };                                                                                            \
                                                                                                  \
    static test::TestGroupBase* ___TEST_TEST_FACTORY_NAME(fixture, test_name)(void* mem)          \
    {                                                                                             \
        return new (mem) ___TEST_TEST_CLASS_NAME(fixture, test_name);                             \
    }                                                                                             \
                                                                                                  \
    const bool ___TEST_TEST_CLASS_NAME(fixture, test_name)::registered_ =                         \
        (AddTest(                                                                                 \
             ___TEST_TEST_NAME(fixture, test_name), ___TEST_TEST_FACTORY_NAME(fixture, test_name) \
         ),                                                                                       \
         true);                                                                                   \
                                                                                                  \
    void ___TEST_TEST_CLASS_NAME(fixture, test_name)::Run_()

// ------------------------------
// User macros
// ------------------------------

#define TEST(test_name) ___TEST_F(TestGroupBase, test_name, false)

#define FAIL_TEST(test_name) ___TEST_F(TestGroupBase, test_name, true)

#define TEST_F(fixture, test_name) ___TEST_F(fixture, test_name, false)

#define FAIL_TEST_F(fixture, test_name) ___TEST_F(fixture, test_name, true)

#endif  // ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_
