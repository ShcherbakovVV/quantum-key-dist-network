#ifndef COMMON_HPP
#define COMMON_HPP

#include <map>
#include <limits>
#include <chrono>
#include <random>
#include <memory>
#include <concepts>

#include "timepoint.hpp"
    
//template< typename Floating >
//concept IsMetricsType = std::floating_point< Floating >;

template <typename K, typename V, typename C>
K& key_of_min_value( const std::map<K, V, C>& m )
{
    std::map<V, K, C> m2 {};
    for (const auto& [key, value] : m)
        m2[value] = key;
    return std::min_element( m2.begin(), m2.end() )->second;
}

namespace dclr 
{
    using IdRep = unsigned int;
    [[maybe_unused]] static constexpr IdRep ID_MAX =
        std::numeric_limits<IdRep>::max();

    using SysClock   = std::chrono::system_clock;
    using Duration   = std::chrono::milliseconds;//TimeDuration::msec;
    using TimeMoment = TimePoint<SysClock, Duration>;

    //template < IsMetricsType MT >
        //using Metrics = MT;
    using Metrics = double;
    
    [[maybe_unused]] static constexpr Metrics METRICS_INFINITY =
        std::numeric_limits<Metrics>::max();
}  // namespace dclr

#endif  // COMMON_HPP

