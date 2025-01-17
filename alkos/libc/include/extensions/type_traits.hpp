#ifndef LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_
#define LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_

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

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

template <class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type         = T;
    using type               = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

#endif  // LIBC_INCLUDE_EXTENSIONS_TYPE_TRAITS_HPP_
