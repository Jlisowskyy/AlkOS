#ifndef ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_
#define ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_

#include <test_module/test_group_base.hpp>
#include <test_module/test_module.hpp>

#define ___TEST_TEST_CLASS_NAME(fixture, test_name) ___test_test_obj_##test_name___743524762436

#define ___TEST_TEST_FACTORY_NAME(fixture, test_name) \
    ___TEST_TEST_CLASS_NAME(fixture, test_name)##_factory

#define ___TEST_TEST_NAME(fixture, test_name) fixture.##test_name

#define TEST_F(fixture, test)                                                             \
    class ___TEST_TEST_CLASS_NAME(fixture, test_name) : public final fixture              \
    {                                                                                     \
        public:                                                                           \
        ___TEST_TEST_CLASS_NAME(test_name)()          = default;                          \
        virtual ~___TEST_TEST_CLASS_NAME(test_name)() = default;                          \
        void Run_() override;                                                             \
                                                                                          \
        private:                                                                          \
        static const bool registered_;                                                    \
    };                                                                                    \
                                                                                          \
    static TestGroupBase* ___TEST_TEST_FACTORY_NAME(fixture, test_name)(void* mem)        \
    {                                                                                     \
        return new (mem) ___TEST_TEST_CLASS_NAME(fixture, test_name);                     \
    }                                                                                     \
                                                                                          \
    const bool ___TEST_TEST_CLASS_NAME(fixture, test_name)::registered_ =                 \
        (test::AddTest(#test_name, ___TEST_TEST_FACTORY_NAME(fixture, test_name)), true); \
                                                                                          \
    void ___TEST_TEST_CLASS_NAME(fixture, test_name)::Run_()

#define TEST(test_name) TEST_F(TestGroupBase, test_name)

#endif  // ALKOS_KERNEL_TEST_TEST_MODULE_MACROS_HPP_
