#ifndef LIBC_INCLUDE_TEMPLATE_LIB_HPP_
#define LIBC_INCLUDE_TEMPLATE_LIB_HPP_

#include <extensions/utility.hpp>
#include <extensions/defines.hpp>

#include <assert.h>

namespace TemplateLib {
    // ------------------------------
    // Rolled Switch
    // ------------------------------

    template<class ExprT, uint64_t kMaxValue, uint64_t kStep, class FuncT, class DefaultFuncT, class... Args>
    FAST_CALL constexpr void RolledSwitch(DefaultFuncT &&default_func, FuncT &&func, const uint64_t value,
                                          Args &&... args) noexcept {
        if (value == kMaxValue) {
            func.template operator()<static_cast<ExprT>(kMaxValue)>(std::forward<Args>(args)...);
            return;
        }

        if constexpr (kMaxValue >= kStep) {
            RolledSwitch<ExprT, kMaxValue - kStep, kStep>(
                std::forward<DefaultFuncT>(default_func),
                std::forward<FuncT>(func),
                value,
                std::forward<Args>(args)...
            );
            return;
        }

        default_func(std::forward<Args>(args)...);
    }

    template<class ExprT, uint64_t kMaxValue, uint64_t kStep, class FuncT, class DefaultFuncT, class... Args>
    FAST_CALL constexpr auto RolledSwitchReturnable(DefaultFuncT &&default_func, FuncT &&func, const uint64_t value,
                                                    Args &&... args) noexcept {
        if (value == kMaxValue) {
            return func.template operator()<static_cast<ExprT>(kMaxValue)>(std::forward<Args>(args)...);
        }

        if constexpr (kMaxValue >= kStep) {
            return RolledSwitchReturnable<ExprT, kMaxValue - kStep, kStep>(
                std::forward<DefaultFuncT>(default_func),
                std::forward<FuncT>(func),
                value,
                std::forward<Args>(args)...
            );
        }

        return default_func(std::forward<Args>(args)...);
    }

    template<class ExprT, uint64_t kMaxValue, uint64_t kStep, class FuncT, class... Args>
    FAST_CALL constexpr void RolledSwitch(FuncT &&func, const uint64_t value, Args &&... args) noexcept {
        RolledSwitch<ExprT, kMaxValue, kStep>(
            []() constexpr FORCE_INLINE_L {
                R_ASSERT_ALWAYS("Switch out of range error...");
            },
            std::forward<FuncT>(func),
            value,
            std::forward<Args>(args)...
        );
    }

    template<class ExprT, uint64_t kMaxValue, uint64_t kStep, class FuncT, class... Args>
    FAST_CALL constexpr auto RolledSwitchReturnable(FuncT &&func, const uint64_t value, Args &&... args) noexcept {
        return RolledSwitchReturnable<ExprT, kMaxValue, kStep>(
            []() constexpr FORCE_INLINE_L {
                R_ASSERT_ALWAYS("Switch out of range error...");
            },
            std::forward<FuncT>(func),
            value,
            std::forward<Args>(args)...
        );
    }

    // ------------------------------
    // Type Checks
    // ------------------------------

    template<typename T, typename... Args>
    constexpr size_t CountType() {
        return (static_cast<size_t>(std::is_same_v<T, Args>) + ...);
    }

    template<typename T, typename... Args>
    constexpr bool HasType() {
        return (std::is_same_v<T, Args> || ...);
    }

    template<typename T, typename... Args>
    constexpr bool HasTypeOnce() {
        return CountType<T, Args...>() == 1;
    }

    template<typename T, typename... Args>
    constexpr bool HasDuplicateType() {
        return CountType<T, Args...>() > 1;
    }

    template<typename... Args>
    constexpr bool HasDuplicateTypes() {
        return (HasDuplicateType<Args, Args...>() || ...);
    }

    // ------------------------------
    // Type List
    // ------------------------------

    template<size_t N, class T, class... Ts>
    struct TypeList {
        static constexpr size_t size = sizeof...(Ts) + 1;

        static_assert(N < size, "Index out of range");
        using type = typename TypeList<N - 1, Ts...>::type;
    };

    template<class T, class... Ts>
    struct TypeList<0, T, Ts...> {
        static constexpr size_t size = sizeof...(Ts) + 1;

        using type = T;
    };

    template<size_t N, template <size_t> class TypeList>
    struct IterateTypeList {
        template<class Callable, class... Args>
        FORCE_INLINE_F static constexpr void Apply(Callable &&func, Args &&... args) {
            func.template operator()<N, typename TypeList<N>::type>(args...);
            IterateTypeList<N - 1, TypeList>::Apply(
                std::forward<Callable>(func),
                std::forward<Args>(args)...
            );
        }
    };

    template<template <size_t> class TypeList>
    struct IterateTypeList<0, TypeList> {
        template<class Callable>
        FAST_CALL constexpr void Apply(Callable &&func) {
            func.template operator()<0, typename TypeList<0>::type>();
        }
    };

    template<class... Args>
    struct IterateTypes {
        static_assert(sizeof...(Args) > 0, "Type list must not be empty");

        template<size_t N>
        using TypeList = TypeList<N, Args...>;

        template<class Callable, class... CallArgs>
        FAST_CALL constexpr void Apply(Callable &&func, CallArgs &&... args) {
            IterateTypeList<sizeof...(Args) - 1, TypeList>::Apply(
                std::forward<Callable>(func),
                std::forward<CallArgs>(args)...
            );
        }
    };

    template<class T, class... Args>
    NODSCRD FAST_CALL constexpr size_t GetTypeIndexInTypes() {
        static_assert(HasTypeOnce<T, Args...>(), "Type must occur exactly once in the tuple");
        size_t idx{};

        IterateTypes<Args...>::Apply([&]<size_t Index, class U>() {
            if constexpr (std::is_same_v<T, U>) {
                idx = Index;
            }
        });

        return idx;
    }
}
#endif // LIBC_INCLUDE_TEMPLATE_LIB_HPP_
