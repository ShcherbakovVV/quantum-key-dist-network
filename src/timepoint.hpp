#ifndef TIMEPOINT_HPP
#define TIMEPOINT_HPP

#include <chrono>

#include <boost/log/trivial.hpp>

/*
struct TimeDuration
{
    using nsec = typename std::chrono::nanoseconds;
    using usec = typename std::chrono::microseconds;
    using msec = typename std::chrono::milliseconds;
    using sec  = typename std::chrono::seconds;
    using min  = typename std::chrono::minutes;
    using hour = typename std::chrono::hours;
    using day  = typename std::chrono::days;
    using week = typename std::chrono::weeks;
    using mon  = typename std::chrono::months;
    using year = typename std::chrono::years;
};
*/

template <typename Clock>
concept IsClockType = std::chrono::is_clock_v<Clock>;

template <IsClockType Clock, typename Duration>
struct TimePoint
{
    using TP = typename std::chrono::time_point<Clock, Duration>;

    using clock_type = Clock;
    using dur_type = Duration;

    TP time_point;

    TimePoint (TP tp)
    :
        time_point {tp}
    {}

    TimePoint ()
    :
        TimePoint { std::chrono::time_point_cast<Duration>(Clock::now()) }
    {}

    static TimePoint<Clock, Duration> now()
    {
        return TimePoint<Clock, Duration> {};
    }

    template <typename Duration2>
    TimePoint<Clock, Duration2> 
    to( const TimePoint< Clock, Duration >& tp ) const
    {
        return TimePoint<Clock, Duration2> 
            { std::chrono::time_point_cast<Duration2>(tp.time_point) };
    }

    template <typename Duration2>
    TimePoint<Clock, std::common_type_t<dur_type, Duration2>>
    operator+ ( const Duration2& d ) const
    {
        return TimePoint<Clock, std::common_type_t<dur_type, Duration2>> 
                                                            { time_point + d };
    }
    
    template <typename Duration2>
    TimePoint<Clock, std::common_type_t<dur_type, Duration2>>
    operator- ( const Duration2& d ) const
    {
        return operator+(-d);
    }
    
    template <typename Duration2>
    bool operator< ( const TimePoint<Clock, Duration2>& tp2 ) const
    {
        return time_point < tp2.time_point;
    }
    
    template <typename Duration2>
    bool operator> ( const TimePoint<Clock, Duration2>& tp2 ) const
    {
        return tp2.time_point < time_point;
    }
    
    template <typename Duration2>
    bool operator== ( const TimePoint<Clock, Duration2>& tp2 ) const
    {
        return time_point == tp2.time_point;
    }
};

template <typename Clock, typename Duration>
std::ostream& 
operator<< ( std::ostream& os, const TimePoint<Clock, Duration>& tp )
{
    return os << tp.time_point.time_since_epoch().count();
}

#endif  // TIMEPOINT_HPP 
