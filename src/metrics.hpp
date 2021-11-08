#ifndef METRICS_HPP
#define METRICS_HPP

#include <concepts>
#include <type_traits>

template <typename T>
struct TypeTagBase
{
    using type = T;
};

template <std::integral I>
struct IntTag : TypeTagBase<I>
{
    using type = I;
};

template <std::floating_point F>
struct FloatTag : TypeTagBase<F>
{
    using type = F;
};

template <typename T>
concept TypeTag = std::is_base_of_v<TypeTagBase<typename T::type>, T>;

struct SignTagBase {};
struct PositiveTag : SignTagBase {};
struct NegativeTag : SignTagBase {};
struct NotPositiveTag : SignTagBase {};
struct NotNegativeTag : SignTagBase {};

template <typename S>
concept SignTag = std::is_base_of_v<SignTagBase, S>;

template <typename T, TypeTag TT, SignTag ST>
struct Metrics
{
    using type = std::conditional< std::is_same_v<T::type, > >;


    Metrics ( T::,  );
};

#endif  // METRICS_HPP
