#ifndef ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_
#define ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_

#include <defines.hpp>
#include <new.hpp>
#include <test_module/test_group_base.hpp>
#include <test_module/test_module.hpp>

/* must be present here, we cannot use '::' operator because we use fixture name directly in the
 * class name */
using test::AddTest;
using test::g_expectFail;
using test::TestGroupBase;

#define ___TEST_TEST_CLASS_NAME(fixture, test_name) ___test_test_obj_##fixture##_##test_name

#define ___TEST_TEST_FACTORY_NAME(fixture, test_name) \
    ___test_test_obj_##fixture##_##test_name##_factory

#define ___TEST_TEST_NAME(fixture, test_name) fixture##_##test_name

#define ___TEST_F(fixture, test_name, expect_fail)                                       \
    class ___TEST_TEST_CLASS_NAME(fixture, test_name) final : public fixture             \
    {                                                                                    \
        public:                                                                          \
        ___TEST_TEST_CLASS_NAME(fixture, test_name)()           = default;               \
        ~___TEST_TEST_CLASS_NAME(fixture, test_name)() override = default;               \
        void SetExpectFail_() override { g_expectFail = expect_fail; }                   \
        void Run_() override;                                                            \
                                                                                         \
        private:                                                                         \
        static const bool registered_;                                                   \
    };                                                                                   \
                                                                                         \
    static TestGroupBase* ___TEST_TEST_FACTORY_NAME(fixture, test_name)(void* mem)       \
    {                                                                                    \
        return new (mem) ___TEST_TEST_CLASS_NAME(fixture, test_name)();                  \
    }                                                                                    \
                                                                                         \
    const bool ___TEST_TEST_CLASS_NAME(fixture, test_name)::registered_ = []() -> bool { \
        AddTest(                                                                         \
            TOSTRING(___TEST_TEST_NAME(fixture, test_name)),                             \
            ___TEST_TEST_FACTORY_NAME(fixture, test_name)                                \
        );                                                                               \
        return true;                                                                     \
    }();                                                                                 \
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
