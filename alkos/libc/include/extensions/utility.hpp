#ifndef LIBC_INCLUDE_EXTENSIONS_UTILITY_HPP_
#define LIBC_INCLUDE_EXTENSIONS_UTILITY_HPP_

#include <extensions/defines.hpp>
#include <extensions/type_traits.hpp>

namespace std
{
/* STL forward */
template <typename T>
NODSCRD FORCE_INLINE_F constexpr T &&forward(remove_reference_t<T> &t) noexcept
{
    return static_cast<T &&>(t);
}

template <typename T>
NODSCRD FORCE_INLINE_F constexpr T &&forward(remove_reference_t<T> &&t) noexcept
{
    static_assert(
        !is_lvalue_reference_v<T>, "std::forward must not be used to convert an rvalue to an lvalue"
    );
    return static_cast<T &&>(t);
}

/* STL move */
template <typename T>
NODSCRD FORCE_INLINE_F constexpr remove_reference_t<T> &&move(T &&t) noexcept
{
    return static_cast<remove_reference_t<T> &&>(t);
}
}  // namespace std
#endif  // LIBC_INCLUDE_EXTENSIONS_UTILITY_HPP_
