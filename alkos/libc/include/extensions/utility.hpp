#ifndef LIBC_INCLUDE_EXTENSIONS_UTILITY_HPP_
#define LIBC_INCLUDE_EXTENSIONS_UTILITY_HPP_

#include <extensions/defines.hpp>
#include <extensions/type_traits.hpp>

/* STL forward */
template <typename T>
NODSCRD constexpr T &&forward(remove_reference_t<T> &t) noexcept
{
    return static_cast<T &&>(t);
}

template <typename T>
NODSCRD constexpr T &&forward(remove_reference_t<T> &&t) noexcept
{
    return static_cast<T &&>(t);
}

/* STL move */
template <typename T>
NODSCRD constexpr remove_reference_t<T> &&move(T &&t) noexcept
{
    return static_cast<remove_reference_t<T> &&>(t);
}

#endif  // LIBC_INCLUDE_EXTENSIONS_UTILITY_HPP_
