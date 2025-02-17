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

namespace std {
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

    // ------------------------------
    // std::remove_reference_t
    // ------------------------------

    template<class T>
    using remove_reference_t = typename remove_reference<T>::type;

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

    template<class T>
    using type_identity_t = typename type_identity<T>::type;

    // ------------------------------
    // std::add_pointer
    // ------------------------------

    template<typename T>
    struct add_pointer {
        using type = remove_reference_t<T> *;
    };

    template<class T>
    using add_pointer_t = typename add_pointer<T>::type;

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

    template<class T>
    using remove_volatile_t = typename remove_volatile<T>::type;

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

    template<class T>
    using remove_const_t = typename remove_const<T>::type;

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

    template<class T>
    using remove_cv_t = typename remove_cv<T>::type;

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

    template<class T>
    using remove_pointer_t = typename remove_pointer<T>::type;

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

    template<class T>
    using remove_extent_t = typename remove_extent<T>::type;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Primary type categories
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::is_void
    // ------------------------------

    template<class T>
    struct is_void : std::is_same<void, std::remove_cv_t<T> > {
    };

    template<class T>
    constexpr bool is_void_v = is_void<T>::value;

    // ------------------------------
    // std::is_null_pointer
    // ------------------------------

    template<class T>
    struct is_null_pointer : std::is_same<std::nullptr_t, std::remove_cv_t<T> > {
    };

    template<class T>
    constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    // ------------------------------
    // std::is_integral
    // ------------------------------

    template<class T>
    struct __is_integral_helper : std::bool_constant<
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

    template<class T>
    struct is_integral : std::__is_integral_helper<std::remove_cv_t<T> > {
    };

    template<class T>
    constexpr bool is_integral_v = is_integral<T>::value;

    // ------------------------------
    // std::is_floating_point
    // ------------------------------

    template<class T>
    struct __is_floating_point_helper : std::bool_constant<
                std::is_same_v<T, float> ||
                std::is_same_v<T, double> ||
                std::is_same_v<T, long double>> {
    };

    template<class T>
    struct is_floating_point : std::__is_floating_point_helper<std::remove_cv_t<T> > {
    };

    template<class T>
    constexpr bool is_floating_point_v = is_floating_point<T>::value;

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

    template<class T>
    constexpr bool is_array_v = is_array<T>::value;

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

    template<class T>
    struct __is_pointer_helper : std::false_type {
    };

    template<class T>
    struct __is_pointer_helper<T *> : std::true_type {
    };

    template<class T>
    struct is_pointer : std::__is_pointer_helper<std::remove_cv_t<T> > {
    };

    template<class T>
    constexpr bool is_pointer_v = std::is_pointer<T>::value;

    // ------------------------------
    // std::is_lvalue_reference
    // ------------------------------

    template<typename>
    struct is_lvalue_reference : false_type {
    };

    template<typename _Tp>
    struct is_lvalue_reference<_Tp &> : true_type {
    };

    template<class T>
    constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

    // ------------------------------
    // std::is_rvalue_reference
    // ------------------------------

    template<typename>
    struct is_rvalue_reference : false_type {
    };

    template<typename _Tp>
    struct is_rvalue_reference<_Tp &&> : true_type {
    };

    template<class T>
    constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

    // ------------------------------
    // std::is_function
    // ------------------------------

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_function, __is_function)

    // -------------------------------------
    // std::is_member_function_pointer
    // -------------------------------------

    template<class T>
    struct __is_member_function_pointer_helper : std::false_type {
    };

    template<class T, class U>
    struct __is_member_function_pointer_helper<T U::*> : std::is_function<T> {
    };

    template<class T>
    struct is_member_function_pointer
            : __is_member_function_pointer_helper<remove_cv_t<T> > {
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Composite Types
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ------------------------------
    // std::is_arithmetic
    // ------------------------------

    template<class T>
    struct is_arithmetic : std::integral_constant<bool,
                std::is_integral<T>::value ||
                std::is_floating_point<T>::value> {
    };

    template<class T>
    constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

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

    template<class T>
    constexpr bool is_fundamental_v = is_fundamental<T>::value;

    // ------------------------------
    // std::is_member_pointer
    // ------------------------------

    template<class T>
    struct __is_member_pointer_helper : std::false_type {
    };

    template<class T, class U>
    struct __is_member_pointer_helper<T U::*> : std::true_type {
    };

    template<class T>
    struct is_member_pointer : __is_member_pointer_helper<std::remove_cv_t<T> > {
    };

    template<class T>
    constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

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

    template<class T>
    constexpr bool is_scalar_v = is_scalar<T>::value;

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

    template<class T>
    constexpr bool is_object_v = is_object<T>::value;

    // ------------------------------
    // std::is_compound
    // ------------------------------

    template<class T>
    struct is_compound : std::bool_constant<!std::is_fundamental_v<T>> {
    };

    template<class T>
    constexpr bool is_compound_v = is_compound<T>::value;

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

    template<class T>
    constexpr bool is_reference_v = is_reference<T>::value;

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

    template<class T>
    constexpr bool is_const_v = is_const<T>::value;

    // ------------------------------
    // std::is_volatile
    // ------------------------------

    template<class T>
    struct is_volatile : std::false_type {
    };

    template<class T>
    struct is_volatile<volatile T> : std::true_type {
    };

    template<class T>
    constexpr bool is_volatile_v = is_volatile<T>::value;

    // ------------------------------
    // std::is_unbounded_array
    // ------------------------------

    template<class T>
    struct is_unbounded_array : std::false_type {
    };

    template<class T>
    struct is_unbounded_array<T[]> : std::true_type {
    };

    template<class T>
    constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

    // ------------------------------
    // std::is_bounded_array
    // ------------------------------

    template<class T>
    struct is_bounded_array : std::false_type {
    };

    template<class T, std::size_t N>
    struct is_bounded_array<T[N]> : std::true_type {
    };

    template<class T>
    constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

    // ------------------------------
    // std::is_unsigned
    // ------------------------------

    template<class T>
    struct is_unsigned : std::bool_constant<
                std::is_arithmetic_v<T> &&
                T(0) < T(-1)
            > {
    };

    template<class T>
    constexpr bool is_unsigned_v = is_unsigned<T>::value;

    // ------------------------------
    // std::is_unsigned
    // ------------------------------

    template<class T>
    struct is_signed : std::bool_constant<
                std::is_arithmetic_v<T> &&
                T(-1) < T(0)
            > {
    };

    template<class T>
    constexpr bool is_signed_v = is_signed<T>::value;

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
    /// Supported operations
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    __DEF_COMPILER_DEF_TYPE_TRAIT(is_constructible, __is_constructible)
    __DEF_COMPILER_DEF_TYPE_TRAIT(is_trivially_constructible, __is_trivially_constructible)
    __DEF_COMPILER_DEF_TYPE_TRAIT(is_nothrow_constructible, __is_nothrow_constructible)


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Transformations section 2
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
