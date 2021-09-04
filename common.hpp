#ifndef COMMON_HPP
#define COMMON_HPP

#include <limits>

namespace common
{
    // тип id различных объектов
    using Id = int;

    // часы
    using SysClock = typename std::chrono::system_clock;
    using Duration = typename std::chrono::milliseconds;

    // тип метрики
    using Metrics = typename double;
    Metrics METRICS_INFINITY = std::numeric_limits< Metrics >::max();

}  // namespace common

#endif  // COMMON_HPP

