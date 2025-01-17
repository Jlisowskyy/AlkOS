#ifndef LIBCXX_INCLUDE_TUPLE_HPP_
#define LIBCXX_INCLUDE_TUPLE_HPP_

#include <stdlib.h>

#include <defines.h>
#include <extensions/type_traits.hpp>
#include <extensions/utility.hpp>

namespace std
{
// ------------------------------
// tuple
// ------------------------------

template <typename T, typename... Args>
struct __BaseTuple {
    static constexpr size_t Size = 1 + sizeof...(Args);

    FORCE_INLINE_F explicit constexpr __BaseTuple(T &&value, Args &&...args)
        : m_value(std::forward<T>(value)), m_next(std::forward<Args>(args)...)
    {
    }

    template <size_t Index>
    NODSCRD FORCE_INLINE_F constexpr const auto &get() const
    {
        static_assert(Index < Size, "Index out of range");
        if constexpr (Index == 0) {
            return m_value;
        } else {
            return m_next.template get<Index - 1>();
        }
    }

    template <size_t Index>
    NODSCRD FORCE_INLINE_F constexpr auto &get()
    {
        static_assert(Index < Size, "Index out of range");
        if constexpr (Index == 0) {
            return m_value;
        } else {
            return m_next.template get<Index - 1>();
        }
    }

    T m_value;
    __BaseTuple<Args...> m_next;
};

template <typename T>
struct __BaseTuple<T> {
    explicit constexpr __BaseTuple(T &&value) : m_value(std::forward<T>(value)) {}

    template <size_t Index>
    NODSCRD FORCE_INLINE_F constexpr const T &get() const
    {
        static_assert(Index == 0, "Index out of range");
        return m_value;
    }

    template <size_t Index>
    NODSCRD FORCE_INLINE_F constexpr T &get()
    {
        static_assert(Index == 0, "Index out of range");
        return m_value;
    }

    T m_value;
};

template <typename... Args>
struct tuple : __BaseTuple<Args...> {
    FORCE_INLINE_F explicit constexpr tuple(Args &&...args)
        : __BaseTuple<Args...>(std::forward<Args>(args)...)
    {
    }
};

// ------------------------------
// make_tuple
// ------------------------------

/* TODO: decay? */
template <typename... Args>
NODSCRD FORCE_INLINE_F constexpr tuple<Args...> make_tuple(Args &&...args)
{
    return tuple<Args...>(std::forward<Args>(args)...);
}

// ------------------------------
// std::tuple_size
// ------------------------------

// NOTE: required for structured bindings
template <class T>
struct tuple_size {
};

template <class... Types>
struct tuple_size<tuple<Types...>> : std::integral_constant<size_t, sizeof...(Types)> {
};

template <class... Types>
struct tuple_size<const tuple<Types...>> : std::integral_constant<size_t, sizeof...(Types)> {
};

template <class T>
constexpr size_t tuple_size_v = tuple_size<T>();

template <typename... _Types>
constexpr size_t tuple_size_v<tuple<_Types...>> = sizeof...(_Types);

template <typename... _Types>
constexpr size_t tuple_size_v<const tuple<_Types...>> = sizeof...(_Types);

// ------------------------------
// std::tuple_element
// ------------------------------
// NOTE: required for structured bindings

template <size_t Index, class T>
struct tuple_element {
};

template <size_t Index, typename T, typename... Args>
struct __TupleElement {
    using type = typename __TupleElement<Index - 1, Args...>::type;
};

template <typename T, typename... Args>
struct __TupleElement<0, T, Args...> {
    using type = T;
};

template <size_t Index, typename... Args>
struct tuple_element<Index, tuple<Args...>> {
    static_assert(Index < sizeof...(Args), "Index out of range");
    using type = typename __TupleElement<Index, Args...>::type;
};

template <size_t Index, typename... Args>
struct tuple_element<Index, const tuple<Args...>> {
    static_assert(Index < sizeof...(Args), "Index out of range");
    using type = typename __TupleElement<Index, Args...>::type;
};

// ------------------------------
// std::get
// ------------------------------
// NOTE: required for structured bindings

template <size_t Index, typename... Args>
NODSCRD FORCE_INLINE_F constexpr auto &get(tuple<Args...> &tup)
{
    return tup.template get<Index>();
}

template <size_t Index, typename... Args>
NODSCRD FORCE_INLINE_F constexpr const auto &get(const tuple<Args...> &tup)
{
    return tup.template get<Index>();
}

template <size_t Index, typename... Args>
NODSCRD FORCE_INLINE_F constexpr auto &&get(tuple<Args...> &&tup)
{
    return std::move(tup).template get<Index>();
}
}  // namespace std

#endif  // LIBCXX_INCLUDE_TUPLE_HPP_
