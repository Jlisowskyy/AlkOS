#ifndef LIBC_INCLUDE_EXTENSIONS_TUPLE_HPP_
#define LIBC_INCLUDE_EXTENSIONS_TUPLE_HPP_

#include <extensions/utility.hpp>

template <typename T, typename... Args>
struct __BaseTuple {
    static constexpr size_t Size = 1 + sizeof...(Args);

    explicit constexpr __BaseTuple(T &&value, Args &&...args)
        : m_value(forward<T>(value)), m_next(forward<Args>(args)...)
    {
    }

    template <size_t Index>
    constexpr const auto &get() const
    {
        static_assert(Index < Size, "Index out of range");

        if constexpr (Index == 0) {
            return m_value;
        } else {
            return m_next.template get<Index - 1>();
        }
    }

    template <size_t Index>
    constexpr auto &get()
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
    explicit constexpr __BaseTuple(T &&value) : m_value(forward<T>(value)) {}

    template <size_t Index>
    constexpr const T &get() const
    {
        static_assert(Index == 0, "Index out of range");
        return m_value;
    }

    template <size_t Index>
    constexpr T &get()
    {
        static_assert(Index == 0, "Index out of range");
        return m_value;
    }

    T m_value;
};

namespace std
{
template <typename... Args>
struct tuple : __BaseTuple<Args...> {
    explicit constexpr tuple(Args &&...args) : __BaseTuple<Args...>(forward<Args>(args)...) {}
};

template <typename... Args>
constexpr tuple<Args...> make_tuple(Args &&...args)
{
    return tuple<Args...>(forward<Args>(args)...);
}

/* Structural binding for tuple */
/* https://en.cppreference.com/w/cpp/language/structured_binding */

/* Requirement 1. there is a std::tuple_size implementation for std::tuple */
template <class... Types>
struct tuple_size<tuple<Types...> > : integral_constant<size_t, sizeof...(Types)> {
};

/* Requirement 2.there is tuple_element specialization for std::tuple */
template <size_t Index, typename T, typename... Args>
struct __TupleElement<Index, tuple<T, Args...> > {
    using type = typename __TupleElement<Index - 1, tuple<Args...> >::type;
};

template <typename T, typename... Args>
struct __TupleElement<0, tuple<T, Args...> > {
    using type = T;
};

template <size_t Index, typename... Args>
struct tuple_element<Index, tuple<Args...> > {
    static_assert(Index < sizeof...(Args), "Index out of range");

    using type = typename __TupleElement<Index, tuple<Args...> >::type;
};

/* Requirement 3. there is implementation of std::get */
template <size_t Index, typename... Args>
constexpr auto &get(tuple<Args...> &tup)
{
    return tup.template get<Index>();
}

template <size_t Index, typename... Args>
constexpr const auto &get(const tuple<Args...> &tup)
{
    return tup.template get<Index>();
}
}  // namespace std

#endif  // LIBC_INCLUDE_EXTENSIONS_TUPLE_HPP_
