#ifndef LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_
#define LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_

#include <extensions/defines.hpp>

namespace std
{
// ------------------------------
// std::remove_reference
// ------------------------------

template <class T>
struct remove_reference {
    using type = T;
};

template <class T>
struct remove_reference<T &> {
    using type = T;
};

template <class T>
struct remove_reference<T &&> {
    using type = T;
};

// ------------------------------
// std::remove_reference_t
// ------------------------------

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

// ------------------------------
// std::integral_constant
// ------------------------------

template <class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type         = T;
    using type               = integral_constant;
    NODSCRD FORCE_INLINE_F constexpr operator value_type() const noexcept { return value; }
    NODSCRD FORCE_INLINE_F constexpr value_type operator()() const noexcept { return value; }
};

// ------------------------------
// bool integral constants
// ------------------------------

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

// ------------------------------
// std::is_same
// ------------------------------

template <class X, class Y>
struct is_same : false_type {
};

template <class X>
struct is_same<X, X> : true_type {
};

template <class X, class Y>
constexpr bool is_same_v = is_same<X, Y>::value;

// ------------------------------
// std::type_identity
// ------------------------------

template <class T>
struct type_identity {
    using type = T;
};

template <class T>
using type_identity_t = typename type_identity<T>::type;

// ------------------------------
// std::add_pointer
// ------------------------------

template <typename T>
struct add_pointer {
    using type = remove_reference_t<T> *;
};

template <class T>
using add_pointer_t = typename add_pointer<T>::type;

// ------------------------------
// std::conditional
// ------------------------------

template <bool B, class X, class Y>
struct conditional {
    using type = X;
};

template <class X, class Y>
struct conditional<false, X, Y> {
    using type = Y;
};

template <bool B, class X, class Y>
using conditional_t = typename conditional<B, X, Y>::type;

// ------------------------------
// std::remove_volatile
// ------------------------------

template <class T>
struct remove_volatile : type_identity<T> {
};

template <class T>
struct remove_volatile<volatile T> {
    using type = T;
};

template <class T>
using remove_volatile_t = typename remove_volatile<T>::type;

// ------------------------------
// std::remove_const
// ------------------------------

template <class T>
struct remove_const : type_identity<T> {
};

template <class T>
struct remove_const<const T> {
    using type = T;
};

template <class T>
using remove_const_t = typename remove_const<T>::type;

// ------------------------------
// std::remove_cv
// ------------------------------

template <class T>
struct remove_cv : type_identity<T> {
};

template <class T>
struct remove_cv<const T> : type_identity<T> {
};

template <class T>
struct remove_cv<volatile T> : type_identity<T> {
};

template <class T>
struct remove_cv<const volatile T> : type_identity<T> {
};

template <class T>
using remove_cv_t = typename remove_cv<T>::type;

// ------------------------------
// std::remove_pointer
// ------------------------------

template <class T>
struct remove_pointer : type_identity<T> {
};

template <class T>
struct remove_pointer<T *> : type_identity<T> {
};

template <class T>
struct remove_pointer<T *const> : type_identity<T> {
};

template <class T>
struct remove_pointer<T *volatile> : type_identity<T> {
};

template <class T>
struct remove_pointer<T *volatile const> : type_identity<T> {
};

template <class T>
using remove_pointer_t = typename remove_pointer<T>::type;

// ------------------------------
// std::remove_extent
// ------------------------------

template <class T>
struct remove_extent : type_identity<T> {
};

template <class T>
struct remove_extent<T[]> : type_identity<T> {
};

template <class T, size_t N>
struct remove_extent<T[N]> : type_identity<T> {
};

template <class T>
using remove_extent_t = typename remove_extent<T>::type;

// ------------------------------
// std::is_const
// ------------------------------

template <class T>
struct is_const : false_type {
};

template <class T>
struct is_const<const T> : true_type {
};

template <class T>
constexpr bool is_const_v = is_const<T>::value;

// ------------------------------
// std::is_reference
// ------------------------------

template <class T>
struct is_reference : false_type {
};

template <class T>
struct is_reference<T &> : true_type {
};

template <class T>
struct is_reference<T &&> : true_type {
};

template <class T>
constexpr bool is_reference_v = is_reference<T>::value;

// ------------------------------
// std::is_function
// ------------------------------

template <typename T>
struct is_function : bool_constant<__is_function(T)> {
};

template <class T>
constexpr bool is_function_v = is_function<T>::value;

// ------------------------------
// std::is_array
// ------------------------------

template <class T>
struct is_array : false_type {
};

template <class T>
struct is_array<T[]> : true_type {
};

template <class T, size_t N>
struct is_array<T[N]> : true_type {
};

template <class T>
constexpr bool is_array_v = is_array<T>::value;

// ------------------------------
// std::decay
// ------------------------------

template <class T>
struct decay {
    private:
    using U = remove_reference_t<T>;

    public:
    using type = conditional_t<
        is_array_v<U>, add_pointer_t<remove_extent_t<U>>,
        conditional_t<is_function_v<U>, add_pointer_t<U>, remove_cv_t<U>>>;
};

template <class T>
using decay_t = typename decay<T>::type;
}  // namespace std

#endif  // LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_
