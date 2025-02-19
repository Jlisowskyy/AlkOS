#ifndef LIBC_INCLUDE_TUPLE_HPP_
#define LIBC_INCLUDE_TUPLE_HPP_

#include <stdlib.h>

#include <defines.h>
#include <extensions/type_traits.hpp>
#include <extensions/utility.hpp>

#include <extensions/template_lib.hpp>

namespace std {
    // ------------------------------
    // tuple
    // ------------------------------

    template<typename T, typename... Args>
    struct __BaseTuple {
        static constexpr size_t Size = 1 + sizeof...(Args);

        /* 1. Default constructor */
        FORCE_INLINE_F constexpr __BaseTuple() = default;

        /* 2. Usual types */
        FORCE_INLINE_F constexpr __BaseTuple(const T &value, const Args &... args)
            : m_value(value), m_next(args...) {
        }

        /* 3. Copy/move constructor */
        FORCE_INLINE_F constexpr __BaseTuple(const __BaseTuple &other) = default;

        FORCE_INLINE_F constexpr __BaseTuple(__BaseTuple &&other) = default;

        /* 4. R-Value construction */
        template<typename U, typename... UArgs>
        FORCE_INLINE_F constexpr __BaseTuple(U &&value, UArgs &&... args)
            : m_value(std::forward<U>(value)), m_next(std::forward<UArgs>(args)...) {
            static_assert(sizeof...(UArgs) + 1 == Size, "Invalid number of arguments");
        }

        /* 5. Other tuple construction */
        template<typename... UArgs>
        FORCE_INLINE_F constexpr __BaseTuple(const __BaseTuple<UArgs...> &other)
            : m_value(other.template get<0>()), m_next(other.m_next) {
            static_assert(sizeof...(UArgs) + 1 == Size, "Invalid number of arguments");
        }

        template<typename U, typename... UArgs>
        FORCE_INLINE_F constexpr __BaseTuple(__BaseTuple<U, UArgs...> &&other)
            : m_value(std::forward<U>(other.template get<0>())),
              m_next(std::forward<__BaseTuple<UArgs...> >(other.m_next)) {
            static_assert(sizeof...(UArgs) + 1 == Size, "Invalid number of arguments");
        }

        // ------------------------------
        // Methods
        // ------------------------------

        template<size_t Index>
        NODSCRD FORCE_INLINE_F constexpr const auto &get() const {
            static_assert(Index < Size, "Index out of range");
            if constexpr (Index == 0) {
                return m_value;
            } else {
                return m_next.template get<Index - 1>();
            }
        }

        template<size_t Index>
        NODSCRD FORCE_INLINE_F constexpr auto &get() {
            static_assert(Index < Size, "Index out of range");
            if constexpr (Index == 0) {
                return m_value;
            } else {
                return m_next.template get<Index - 1>();
            }
        }

    protected:
        T m_value{};
        __BaseTuple<Args...> m_next{};
    };

    template<typename T>
    struct __BaseTuple<T> {
        /* 1. Default constructor */
        FORCE_INLINE_F constexpr __BaseTuple() = default;

        /* 2. Usual types */
        FORCE_INLINE_F constexpr __BaseTuple(const T &value)
            : m_value(value) {
        }

        /*3. Copy/move constructor */
        FORCE_INLINE_F constexpr __BaseTuple(const __BaseTuple &other) = default;

        FORCE_INLINE_F constexpr __BaseTuple(__BaseTuple &&other) = default;

        /* 4. R-Value construction */
        template<typename U>
        FORCE_INLINE_F constexpr __BaseTuple(U &&value)
            : m_value(std::forward<U>(value)) {
        }

        /* 5. Other tuple construction */
        template<typename U>
        FORCE_INLINE_F constexpr __BaseTuple(const __BaseTuple<U> &other)
            : m_value(other.template get<0>()) {
        }

        template<typename U>
        FORCE_INLINE_F constexpr __BaseTuple(__BaseTuple<U> &&other)
            : m_value(std::move(other.template get<0>())) {
        }

        // ------------------------------
        // Methods
        // ------------------------------

        template<size_t Index>
        NODSCRD FORCE_INLINE_F constexpr const T &get() const {
            static_assert(Index == 0, "Index out of range");
            return m_value;
        }

        template<size_t Index>
        NODSCRD FORCE_INLINE_F constexpr T &get() {
            static_assert(Index == 0, "Index out of range");
            return m_value;
        }

    protected:
        T m_value;
    };

    template<typename... Args>
    struct tuple : __BaseTuple<Args...> {
        /* 1. Default constructor */
        FORCE_INLINE_F constexpr tuple(): __BaseTuple<Args...>() {
        };

        /* 2. Usual types */
        FORCE_INLINE_F constexpr tuple(const Args &... args): __BaseTuple<Args...>(args...) {
        }

        /* 3. Copy/move constructor */
        FORCE_INLINE_F constexpr tuple(const tuple &other) = default;

        FORCE_INLINE_F constexpr tuple(tuple &&other) = default;

        /* 4. R-Value construction */
        template<typename... UArgs>
        FORCE_INLINE_F constexpr tuple(UArgs &&... args) : __BaseTuple<Args...>(std::forward<UArgs>(args)...) {
            static_assert(sizeof...(UArgs) == sizeof...(Args), "Invalid number of arguments");
        }

        /* 5. Other tuple construction */
        template<typename... UArgs>
        FORCE_INLINE_F constexpr tuple(const tuple<UArgs...> &other): __BaseTuple<Args...>(other) {
            static_assert(sizeof...(UArgs) == sizeof...(Args), "Invalid number of arguments");
        }

        template<typename... UArgs>
        FORCE_INLINE_F constexpr tuple(tuple<UArgs...> &&other): __BaseTuple<Args...>(std::forward<Args...>(other)) {
            static_assert(sizeof...(UArgs) == sizeof...(Args), "Invalid number of arguments");
        }
    };

    // ------------------------------
    // make_tuple
    // ------------------------------

    template<typename... Args>
    NODSCRD FORCE_INLINE_F constexpr tuple<remove_reference_t<decay_t<Args> >...> make_tuple(Args &&... args) {
        using return_tuple_t = tuple<remove_reference_t<decay_t<Args> >...>;

        return return_tuple_t(std::forward<Args>(args)...);
    }

    // ------------------------------
    // std::tuple_size
    // ------------------------------

    // NOTE: required for structured bindings
    template<class T>
    struct tuple_size {
    };

    template<class... Types>
    struct tuple_size<tuple<Types...> > : std::integral_constant<size_t, sizeof...(Types)> {
    };

    template<class... Types>
    struct tuple_size<const tuple<Types...>> : std::integral_constant<size_t, sizeof...(Types)> {
    };

    template<class T>
    constexpr size_t tuple_size_v = tuple_size<T>();

    template<typename... _Types>
    constexpr size_t tuple_size_v<tuple<_Types...> > = sizeof...(_Types);

    template<typename... _Types>
    constexpr size_t tuple_size_v<const tuple<_Types...>> = sizeof...(_Types);

    // ------------------------------
    // std::tuple_element
    // ------------------------------
    // NOTE: required for structured bindings

    template<size_t Index, typename T, typename... Args>
    struct __TupleElement {
        using type = typename __TupleElement<Index - 1, Args...>::type;
    };

    template<typename T, typename... Args>
    struct __TupleElement<0, T, Args...> {
        using type = T;
    };

    template<size_t Index, class T>
    struct tuple_element {
    };

    template<size_t Index, typename... Args>
    struct tuple_element<Index, tuple<Args...> > {
        static_assert(Index < sizeof...(Args), "Index out of range");
        using type = typename __TupleElement<Index, Args...>::type;
    };

    template<size_t Index, typename... Args>
    struct tuple_element<Index, const tuple<Args...>> {
        static_assert(Index < sizeof...(Args), "Index out of range");
        using type = typename __TupleElement<Index, Args...>::type;
    };

    // ------------------------------
    // std::get - indexed
    // ------------------------------
    // NOTE: required for structured bindings

    template<size_t Index, typename... Args>
    NODSCRD FORCE_INLINE_F constexpr
    typename tuple_element<Index, tuple<Args...> >::type &
    get(tuple<Args...> &tup) noexcept {
        return tup.template get<Index>();
    }

    template<size_t Index, typename... Args>
    NODSCRD FORCE_INLINE_F constexpr
    const typename tuple_element<Index, tuple<Args...> >::type &
    get(const tuple<Args...> &tup) noexcept {
        return tup.template get<Index>();
    }

    template<size_t Index, typename... Args>
    NODSCRD FORCE_INLINE_F constexpr
    typename tuple_element<Index, tuple<Args...> >::type &&
    get(tuple<Args...> &&tup) noexcept {
        using element_type = typename tuple_element<Index, tuple<Args...> >::type;
        return forward<element_type>(tup.template get<Index>());
    }

    template<size_t Index, typename... Args>
    NODSCRD FORCE_INLINE_F constexpr
    const typename tuple_element<Index, tuple<Args...> >::type &&
    get(const tuple<Args...> &&tup) noexcept {
        using element_type = typename tuple_element<Index, tuple<Args...> >::type;
        return forward<element_type>(tup.template get<Index>());
    }

    // ------------------------------
    // std::get typed
    // ------------------------------

    template<class T, class... Args>
    NODSCRD FORCE_INLINE_F constexpr
    T &get(tuple<Args...> &tuple) noexcept {
        static_assert(TemplateLib::HasTypeOnce<T, Args...>(), "Type must occur exactly once in the tuple");
        constexpr size_t index = TemplateLib::GetTypeIndexInTypes<T, Args...>();
        return get<index>(tuple);
    }

    template<class T, class... Args>
    NODSCRD FORCE_INLINE_F constexpr
    const T &get(const tuple<Args...> &tuple) noexcept {
        static_assert(TemplateLib::HasTypeOnce<T, Args...>(), "Type must occur exactly once in the tuple");
        constexpr size_t index = TemplateLib::GetTypeIndexInTypes<T, Args...>();
        return get<index>(tuple);
    }

    template<class T, class... Args>
    NODSCRD FORCE_INLINE_F constexpr
    T &&get(tuple<Args...> &&tuple) noexcept {
        static_assert(TemplateLib::HasTypeOnce<T, Args...>(), "Type must occur exactly once in the tuple");
        constexpr size_t index = TemplateLib::GetTypeIndexInTypes<T, Args...>();
        return get<index>(tuple);
    }

    template<class T, class... Args>
    NODSCRD FORCE_INLINE_F constexpr
    const T &&get(const tuple<Args...> &&tuple) noexcept {
        static_assert(TemplateLib::HasTypeOnce<T, Args...>(), "Type must occur exactly once in the tuple");
        constexpr size_t index = TemplateLib::GetTypeIndexInTypes<T, Args...>();
        return get<index>(tuple);
    }
} // namespace std

#endif  // LIBC_INCLUDE_TUPLE_HPP_
