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
    typedef T type;
};

template <class T>
struct remove_reference<T &> {
    typedef T type;
};

template <class T>
struct remove_reference<T &&> {
    typedef T type;
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
// bool integrals
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

}  // namespace std

#endif  // LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_
