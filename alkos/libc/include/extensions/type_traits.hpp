#ifndef LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_
#define LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_

#include <extensions/defines.hpp>
#include <extensions/cstddef.hpp>

#include "type_traits.hpp"

#define __DEF_COMPILER_DEF_TYPE_TRAIT(name, func) \
    template<typename T> \
    struct name : bool_constant<func(T)> { \
    };\
 \
template<class T>\
constexpr bool name##_v = name<T>::value;

#define __DEF_CONSTEXPR_ACCESSOR(name) \
    template<class T> \
    constexpr bool name##_v = name<T>::value;

#define __DEF_CONSTEXPR_ACCESSOR_T(name) \
    template<class T> \
    using name##_t = typename name<T>::type;

namespace std {

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Helper Classes
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::integral_constant
    // ------------------------------

    template<class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant;
        NODSCRD FORCE_INLINE_F constexpr operator value_type() const noexcept { return value; }
        NODSCRD FORCE_INLINE_F constexpr value_type operator()() const noexcept { return value; }
    };

    // ------------------------------
    // bool integral constants
    // ------------------------------

    template<bool B>
    using bool_constant = integral_constant<bool, B>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Miscellaneous transformations
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::is_same
    // ------------------------------

    template<class X, class Y>
    struct is_same : false_type {
    };

    template<class X>
    struct is_same<X, X> : true_type {
    };

    template<class X, class Y>
    constexpr bool is_same_v = is_same<X, Y>::value;

    // ------------------------------
    // std::type_identity
    // ------------------------------

    template<class T>
    struct type_identity {
        using type = T;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(type_identity)

    // ------------------------------
    // std::conditional
    // ------------------------------

    template<bool B, class X, class Y>
    struct conditional {
        using type = X;
    };

    template<class X, class Y>
    struct conditional<false, X, Y> {
        using type = Y;
    };

    template<bool B, class X, class Y>
    using conditional_t = typename conditional<B, X, Y>::type;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Const-volatility specifiers
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::remove_cv
    // ------------------------------

    template<class T>
    struct remove_cv : type_identity<T> {
    };

    template<class T>
    struct remove_cv<const T> : type_identity<T> {
    };

    template<class T>
    struct remove_cv<volatile T> : type_identity<T> {
    };

    template<class T>
    struct remove_cv<const volatile T> : type_identity<T> {
    };

    __DEF_CONSTEXPR_ACCESSOR_T(remove_cv)

    // ------------------------------
    // std::remove_const
    // ------------------------------

    template<class T>
    struct remove_const : type_identity<T> {
    };

    template<class T>
    struct remove_const<const T> {
        using type = T;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(remove_const)

    // ------------------------------
    // std::remove_volatile
    // ------------------------------

    template<class T>
    struct remove_volatile : type_identity<T> {
    };

    template<class T>
    struct remove_volatile<volatile T> {
        using type = T;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(remove_volatile)

    // ------------------------------
    // std::add_cv
    // ------------------------------

    template<class T>
    struct add_cv {
        using type = const volatile T;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(add_cv)

    // ------------------------------
    // std::add_const
    // ------------------------------

    template<class T>
    struct add_const {
        using type = const T;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(add_const)

    // ------------------------------
    // std::add_volatile
    // ------------------------------

    template<class T>
    struct add_volatile {
        using type = volatile T;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(add_volatile)

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// References
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace internal {
        template<class T>
        auto try_add_lvalue_reference(int) -> type_identity<T &> {
            return {};
        }

        template<class T>
        auto try_add_lvalue_reference(...) -> type_identity<T> {
            return {};
        }

        template<class T>
        auto try_add_rvalue_reference(int) -> type_identity<T &&> {
            return {};
        }

        template<class T>
        auto try_add_rvalue_reference(...) -> type_identity<T> {
            return {};
        }
    }

    // ------------------------------
    // std::remove_reference
    // ------------------------------

    template<class T>
    struct remove_reference {
        using type = T;
    };

    template<class T>
    struct remove_reference<T &> {
        using type = T;
    };

    template<class T>
    struct remove_reference<T &&> {
        using type = T;
    };

    template<class T>
    using remove_reference_t = typename remove_reference<T>::type;

    // -------------------------------
    // std::add_lvalue_reference
    // -------------------------------

    template<class T>
    struct add_lvalue_reference : decltype(internal::try_add_lvalue_reference<T>(0)) {
    };

    __DEF_CONSTEXPR_ACCESSOR_T(add_lvalue_reference)

    // -------------------------------
    // std::add_rvalue_reference
    // -------------------------------

    template<class T>
    struct add_rvalue_reference : decltype(internal::try_add_rvalue_reference<T>(0)) {
    };

    __DEF_CONSTEXPR_ACCESSOR_T(add_rvalue_reference)

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Pointers
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::remove_pointer
    // ------------------------------

    template<class T>
    struct remove_pointer : type_identity<T> {
    };

    template<class T>
    struct remove_pointer<T *> : type_identity<T> {
    };

    template<class T>
    struct remove_pointer<T *const> : type_identity<T> {
    };

    template<class T>
    struct remove_pointer<T *volatile> : type_identity<T> {
    };

    template<class T>
    struct remove_pointer<T *volatile const> : type_identity<T> {
    };

    __DEF_CONSTEXPR_ACCESSOR_T(remove_pointer)

    // ------------------------------
    // std::add_pointer
    // ------------------------------

    template<typename T>
    struct add_pointer {
        using type = remove_reference_t<T> *;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(add_pointer)

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Primary type categories
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::is_void
    // ------------------------------

    template<class T>
    struct is_void : std::is_same<void, std::remove_cv_t<T> > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_void)

    // ------------------------------
    // std::is_null_pointer
    // ------------------------------

    template<class T>
    struct is_null_pointer : std::is_same<std::nullptr_t, std::remove_cv_t<T> > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_null_pointer)

    // ------------------------------
    // std::is_integral
    // ------------------------------

    namespace internal {
        template<class T>
        struct is_integral : std::bool_constant<
                    std::is_same_v<T, int> ||
                    std::is_same_v<T, bool> ||
                    std::is_same_v<T, char> ||
                    std::is_same_v<T, signed char> ||
                    std::is_same_v<T, short> ||
                    std::is_same_v<T, long> ||
                    std::is_same_v<T, long long> ||
                    std::is_same_v<T, char8_t> ||
                    std::is_same_v<T, char16_t> ||
                    std::is_same_v<T, char32_t> ||
                    std::is_same_v<T, wchar_t> ||
                    std::is_same_v<T, unsigned int> ||
                    std::is_same_v<T, unsigned char> ||
                    std::is_same_v<T, unsigned short> ||
                    std::is_same_v<T, unsigned long> ||
                    std::is_same_v<T, unsigned long long>> {
        };
    }


    template<class T>
    struct is_integral : internal::is_integral<std::remove_cv_t<T> > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_integral)

    // ------------------------------
    // std::is_floating_point
    // ------------------------------

    namespace internal {
        template<class T>
        struct is_floating_point : std::bool_constant<
                    std::is_same_v<T, float> ||
                    std::is_same_v<T, double> ||
                    std::is_same_v<T, long double>> {
        };
    }

    template<class T>
    struct is_floating_point : internal::is_floating_point<std::remove_cv_t<T> > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_floating_point)

    // ------------------------------
    // std::is_array
    // ------------------------------

    template<class T>
    struct is_array : false_type {
    };

    template<class T>
    struct is_array<T[]> : true_type {
    };

    template<class T, size_t N>
    struct is_array<T[N]> : true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_array)

    // ------------------------------
    // std::is_enum
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_enum, __is_enum)

    // ------------------------------
    // std::is_union
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_union, __is_union)

    // ------------------------------
    // std::is_class
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_class, __is_class)

    // ------------------------------
    // std::is_pointer
    // ------------------------------

    namespace internal {
        template<class T>
        struct is_pointer : std::false_type {
        };

        template<class T>
        struct is_pointer<T *> : std::true_type {
        };
    }

    template<class T>
    struct is_pointer : internal::is_pointer<std::remove_cv_t<T> > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_pointer)

    // ------------------------------
    // std::is_lvalue_reference
    // ------------------------------

    template<typename>
    struct is_lvalue_reference : false_type {
    };

    template<typename _Tp>
    struct is_lvalue_reference<_Tp &> : true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_lvalue_reference)

    // ------------------------------
    // std::is_rvalue_reference
    // ------------------------------

    template<typename>
    struct is_rvalue_reference : false_type {
    };

    template<typename _Tp>
    struct is_rvalue_reference<_Tp &&> : true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_rvalue_reference)

    // ------------------------------
    // std::is_function
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_function, __is_function)

    // -------------------------------------
    // std::is_member_function_pointer
    // -------------------------------------

    namespace internal {
        template<class T>
        struct is_member_function_pointer : std::false_type {
        };

        template<class T, class U>
        struct is_member_function_pointer<T U::*> : std::is_function<T> {
        };
    }

    template<class T>
    struct is_member_function_pointer
            : internal::is_member_function_pointer<remove_cv_t<T> > {
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Composite Types
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::is_arithmetic
    // ------------------------------

    template<class T>
    struct is_arithmetic : std::integral_constant<bool,
                std::is_integral_v<T> ||
                std::is_floating_point_v<T>> {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_arithmetic)

    // ------------------------------
    // std::is_fundamental
    // ------------------------------

    template<class T>
    struct is_fundamental
            : std::bool_constant<
                std::is_arithmetic_v<T> ||
                std::is_void_v<T> ||
                std::is_null_pointer_v<T>
            > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_fundamental)

    // ------------------------------
    // std::is_member_pointer
    // ------------------------------

    namespace internal {
        template<class T>
        struct is_member_pointer : std::false_type {
        };

        template<class T, class U>
        struct is_member_pointer<T U::*> : std::true_type {
        };
    }

    template<class T>
    struct is_member_pointer : internal::is_member_pointer<std::remove_cv_t<T> > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_member_pointer)

    // ------------------------------
    // std::is_scalar
    // ------------------------------

    template<class T>
    struct is_scalar : std::integral_constant<bool, std::is_arithmetic_v<T>
                                                    || std::is_enum_v<T>
                                                    || std::is_pointer_v<T>
                                                    || std::is_member_pointer_v<T>
                                                    || std::is_null_pointer_v<T>> {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_scalar)

    // ------------------------------
    // std::is_object
    // ------------------- -----------

    template<class T>
    struct is_object : std::bool_constant<
                std::is_scalar_v<T> ||
                std::is_array_v<T> ||
                std::is_union_v<T> ||
                std::is_class_v<T>> {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_object)

    // ------------------------------
    // std::is_compound
    // ------------------------------

    template<class T>
    struct is_compound : std::bool_constant<!std::is_fundamental_v<T>> {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_compound)

    // ------------------------------
    // std::is_reference
    // ------------------------------

    template<class T>
    struct is_reference : false_type {
    };

    template<class T>
    struct is_reference<T &> : true_type {
    };

    template<class T>
    struct is_reference<T &&> : true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_reference)

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Type properties
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::is_const
    // ------------------------------

    template<class T>
    struct is_const : false_type {
    };

    template<class T>
    struct is_const<const T> : true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_const)

    // ------------------------------
    // std::is_volatile
    // ------------------------------

    template<class T>
    struct is_volatile : std::false_type {
    };

    template<class T>
    struct is_volatile<volatile T> : std::true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_volatile)

    // ------------------------------
    // std::is_unbounded_array
    // ------------------------------

    template<class T>
    struct is_unbounded_array : std::false_type {
    };

    template<class T>
    struct is_unbounded_array<T[]> : std::true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_unbounded_array)

    // ------------------------------
    // std::is_bounded_array
    // ------------------------------

    template<class T>
    struct is_bounded_array : std::false_type {
    };

    template<class T, std::size_t N>
    struct is_bounded_array<T[N]> : std::true_type {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_bounded_array)

    // ------------------------------
    // std::is_unsigned
    // ------------------------------

    template<class T>
    struct is_unsigned : std::bool_constant<
                std::is_arithmetic_v<T> &&
                T(0) < T(-1)
            > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_unsigned)

    // ------------------------------
    // std::is_unsigned
    // ------------------------------

    template<class T>
    struct is_signed : std::bool_constant<
                std::is_arithmetic_v<T> &&
                T(-1) < T(0)
            > {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_signed)

    // ------------------------------
    // std::is_final
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_final, __is_final)

    // ------------------------------
    // std::is_abstract
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_abstract, __is_abstract)

    // ------------------------------
    // std::is_polymorphic
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_polymorphic, __is_polymorphic)

    // ------------------------------
    // std::is_empty
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_empty, __is_empty)

    // ------------------------------
    // std::is_aggregate
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_aggregate, __is_aggregate)

    // --------------------------------------------
    // std::has_unique_object_representations
    // --------------------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(has_unique_object_representations, __has_unique_object_representations)

    // ------------------------------
    // std::is_standard_layout
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_standard_layout, __is_standard_layout)

    // --------------------------------
    // std::is_trivially_copyable
    // --------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_trivially_copyable, __is_trivially_copyable)

    // ------------------------------
    // std::is_trivial
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_trivial, __is_trivial)

    // ------------------------------
    // std::is_pod
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_pod, __is_pod)

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sign modifiers
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace internal {
        template<class T, bool IsConst, bool IsVolatile>
        struct preserve_cv_base {
            using type = T;
        };

        template<class T>
        struct preserve_cv_base<T, true, false> {
            using type = const T;
        };

        template<class T>
        struct preserve_cv_base<T, false, true> {
            using type = volatile T;
        };

        template<class T>
        struct preserve_cv_base<T, true, true> {
            using type = const volatile T;
        };

        template<class QualifiedT, class UnqualifiedT>
        struct preserve_cv {
            using type = typename preserve_cv_base<UnqualifiedT, std::is_const_v<QualifiedT>, std::is_volatile_v<
                QualifiedT> >::type;
        };
    }

    // ------------------------------
    // std::make_signed
    // ------------------------------

    namespace internal {
        template<class T>
        struct make_signed_base {
            using type = T;
        };

        template<>
        struct make_signed_base<unsigned char> {
            using type = signed char;
        };

        template<>
        struct make_signed_base<unsigned short> {
            using type = short;
        };

        template<>
        struct make_signed_base<unsigned int> {
            using type = int;
        };

        template<>
        struct make_signed_base<unsigned long> {
            using type = long;
        };

        template<>
        struct make_signed_base<unsigned long long> {
            using type = long long;
        };

        template<>
        struct make_signed_base<char> {
            using type = signed char;
        };

        template<
            class T,
            bool IsIntegral = std::is_integral_v<T>,
            bool IsEnum = std::is_enum_v<T>
        >
        struct make_signed;

        template<class T>
        struct make_signed<T, true, false> {
        private:
            using signed_t = typename make_signed_base<std::remove_cv_t<T> >::type;

        public:
            using type = typename preserve_cv<T, signed_t>::type;
        };

        template<class T>
        struct make_signed<T, false, true> {
        };
    }

    // TODO

    // ------------------------------
    // std::make_unsigned
    // ------------------------------

    // TODO

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Arrays
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::remove_extent
    // ------------------------------

    template<class T>
    struct remove_extent : type_identity<T> {
    };

    template<class T>
    struct remove_extent<T[]> : type_identity<T> {
    };

    template<class T, size_t N>
    struct remove_extent<T[N]> : type_identity<T> {
    };

    __DEF_CONSTEXPR_ACCESSOR_T(remove_extent)

    // ------------------------------
    // std::remove_all_extents
    // ------------------------------

    template<class T>
    struct remove_all_extents {
        using type = T;
    };

    template<class T>
    struct remove_all_extents<T[]> {
        using type = typename remove_all_extents<T>::type;
    };

    template<class T, std::size_t N>
    struct remove_all_extents<T[N]> {
        using type = typename remove_all_extents<T>::type;
    };

    __DEF_CONSTEXPR_ACCESSOR_T(remove_all_extents)

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Supported operations
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * TODO: Added static assert for complete type
     */

    // ------------------------------
    // Type construction
    // ------------------------------

    #define __DEF_COMPLETE_ARGS_TYPE_TRAIT(name, func) \
        template<class T, class... Args> \
        struct name : std::bool_constant<func(T, Args...)> { \
        }; \
        \
        template<class T, class... Args> \
        constexpr bool name##_v = name<T, Args...>::value;

    __DEF_COMPLETE_ARGS_TYPE_TRAIT(is_constructible, __is_constructible)
    __DEF_COMPLETE_ARGS_TYPE_TRAIT(is_trivially_constructible, __is_trivially_constructible)
    __DEF_COMPLETE_ARGS_TYPE_TRAIT(is_nothrow_constructible, __is_nothrow_constructible)

    // -------------------------------
    // Type default construction
    // -------------------------------

    template<class T>
    struct is_default_constructible : std::is_constructible<T> {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_default_constructible)

    template<class T>
    struct is_trivially_default_constructible : std::is_trivially_constructible<T> {
    };

    __DEF_CONSTEXPR_ACCESSOR(is_trivially_default_constructible)

    template<class T>
    struct is_nothrow_default_constructible : std::is_nothrow_constructible<T> {
    };

    // ------------------------------
    // Type copy construction
    // ------------------------------


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Miscellaneous transformations 2
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::decay
    // ------------------------------

    // clang-format off
    template <class T>
    struct decay {
    private:
        using U = remove_reference_t<T>;

    public:
        using type = conditional_t<
            is_array_v<U>,
            add_pointer_t<remove_extent_t<U>>,
            conditional_t<
                is_function_v<U>,
                add_pointer_t<U>,
                remove_cv_t<U>
            >
        >;
    };
    // clang-format on

    template<class T>
    using decay_t = typename decay<T>::type;
} // namespace std

#endif  // LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_
