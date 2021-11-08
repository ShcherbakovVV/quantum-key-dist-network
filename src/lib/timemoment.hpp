#ifndef TIMEMOMENT_HPP
#define TIMEMOMENT_HPP

#include <chrono>
#include <iostream>
#include <type_traits>

template <typename C>
concept ClockType = std::chrono::is_clock_v<C>;

template <typename T>
struct IsDurationType : std::false_type {};

template <typename Rep, typename Period>
struct IsDurationType<std::chrono::duration<Rep, Period>> : std::true_type {};

template <typename D>
concept DurationType = IsDurationType<D>::value;

template <ClockType C, DurationType D>
struct TimeMoment
{
    using this_type = TimeMoment<C, D>;
    using clock_type = C;
    using time_duration_type = D;

    using TP = typename std::chrono::time_point<C, D>;
    TP time_point;

    TimeMoment ()
    :
        TimeMoment { C::now() }
    {}

    template <DurationType D2>
    TimeMoment ( std::chrono::time_point<C, D2> tp2 )
    :
        time_point { std::chrono::time_point_cast<D>( tp2 ) }
    {}

    template <DurationType D2>
    TimeMoment ( const TimeMoment<C, D2>& tm2 )
    :
        TimeMoment { tm2.time_point }
    {}

    template <DurationType D2>
    TimeMoment<C, std::common_type_t<D, D2>>
    operator+ ( const D2& d ) const
    {
        using CommonDuration = std::common_type_t<D, D2>;
        return TimeMoment<C, CommonDuration> { time_point + d };
    }

    template <DurationType D2>
    TimeMoment<C, std::common_type_t<D, D2>> operator- ( const D2& d ) const
    {
        return operator+(-d);
    }

    template <DurationType D2>
    bool operator< ( const TimeMoment<C, D2>& tm2 ) const
    {
        return time_point < tm2.time_point;
    }

    template <DurationType D2>
    bool operator> ( const TimeMoment<C, D2>& tm2 ) const
    {
        return tm2.time_point < time_point;
    }

    template <DurationType D2>
    bool operator== ( const TimeMoment<C, D2>& tm2 ) const
    {
        return time_point == tm2.time_point;
    }
};

template <ClockType C, DurationType D>
std::ostream& operator<< ( std::ostream& os, const TimeMoment<C, D>& tm )
{
    return os << tm.time_point.time_since_epoch().count();
}

#endif  // TIMEMOMENT_HPP
