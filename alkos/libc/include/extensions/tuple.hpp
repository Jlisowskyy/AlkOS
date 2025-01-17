#ifndef LIBCXX_INCLUDE_TUPLE_HPP_
#define LIBCXX_INCLUDE_TUPLE_HPP_

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

    explicit constexpr __BaseTuple(T &&value, Args &&...args)
        : m_value(std::forward<T>(value)), m_next(std::forward<Args>(args)...)
    {
    }

    template <size_t Index>
    NODSCRD constexpr const auto &get() const
    {
        static_assert(Index < Size, "Index out of range");
        if constexpr (Index == 0) {
            return m_value;
        } else {
            return m_next.template get<Index - 1>();
        }
    }

    template <size_t Index>
    NODSCRD constexpr auto &get()
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
    NODSCRD constexpr const T &get() const
    {
        static_assert(Index == 0, "Index out of range");
        return m_value;
    }

    template <size_t Index>
    NODSCRD constexpr T &get()
    {
        static_assert(Index == 0, "Index out of range");
        return m_value;
    }

    T m_value;
};

template <typename... Args>
struct tuple : __BaseTuple<Args...> {
    explicit constexpr tuple(Args &&...args) : __BaseTuple<Args...>(std::forward<Args>(args)...) {}
};

// ------------------------------
// make_tuple
// ------------------------------

/* TODO: decay? */
template <typename... Args>
NODSCRD constexpr tuple<Args...> make_tuple(Args &&...args)
{
    return tuple<Args...>(std::forward<Args>(args)...);
}

// ------------------------------
// std::tuple_size
// ------------------------------

// NOTE: required for structured bindings
template <typename... Args>
struct tuple_size<tuple<Args...> > : std::integral_constant<size_t, sizeof...(Args)> {
};

// ------------------------------
// std::tuple_element
// ------------------------------

template <size_t Index, typename T, typename... Args>
struct __TupleElement {
    using type = typename __TupleElement<Index - 1, Args...>::type;
};

template <typename T, typename... Args>
struct __TupleElement<0, T, Args...> {
    using type = T;
};

// NOTE: required for structured bindings
template <size_t Index, typename... Args>
struct tuple_element<Index, tuple<Args...> > {
    static_assert(Index < sizeof...(Args), "Index out of range");
    using type = typename __TupleElement<Index, Args...>::type;
};

// ------------------------------
// std::get
// ------------------------------
// NOTE: required for structured bindings

template <size_t Index, typename... Args>
NODSCRD constexpr auto &get(tuple<Args...> &tup)
{
    return tup.template get<Index>();
}

template <size_t Index, typename... Args>
NODSCRD constexpr const auto &get(const tuple<Args...> &tup)
{
    return tup.template get<Index>();
}

template <size_t Index, typename... Args>
NODSCRD constexpr auto &&get(tuple<Args...> &&tup)
{
    return std::move(tup).template get<Index>();
}
}  // namespace std

#endif  // LIBCXX_INCLUDE_TUPLE_HPP_
