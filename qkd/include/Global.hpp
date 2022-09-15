#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <chrono>
#include <concepts>
#include <ctime>
#include <ios>
#include <limits>
#include <locale>
#include <random>
#include <sstream>


// TYPEDEFS
using TimePoint = unsigned;
using Duration  = unsigned;
using Distance  = double;

using KeyAmount = double;
using KeyRate   = double;

using Metrics = double;


class Clock
{
    friend class Network;

private:
    static inline TimePoint _now = 0;

public:
    static TimePoint now() { return Clock::_now; }
};


class SetDecimalPoint final : public std::numpunct<char> 
{
private:
    char _decimal_point;
    
public:
    SetDecimalPoint(char decpoint = '.')
    :
        _decimal_point {decpoint}
    {}
    
    char do_decimal_point() const { return _decimal_point; }
};
// END OF TYPEDEFS


// CONSTANTS
constexpr KeyRate QUANTUM_KEY_LENGTH_BITS = 256;  // move to key generation model
constexpr Metrics METRICS_INFINITY = std::numeric_limits<Metrics>::max();
// END OF CONSTANTS


// FUNCTIONS
template<typename Clk>
std::string time_point_to_str(std::chrono::time_point<Clk> tp)
{
    std::chrono::hh_mm_ss time {tp.time_since_epoch()};
    
    long hours = time.hours().count();
    long minutes = time.minutes().count();
    long seconds = time.seconds().count();
    
    auto this_day = std::chrono::duration_cast<std::chrono::days>(tp.time_since_epoch());
    long hour_of_this_day = hours - (std::chrono::hh_mm_ss {this_day}).hours().count();
    
    char hhmmss[9];
    std::snprintf(&hhmmss[0], 9, "%.2ld:%.2ld:%.2ld", hour_of_this_day, minutes, seconds);

    return {hhmmss};
}


template<typename T1, typename T2>
bool pair_contains(const std::pair<T1, T2>& pair, T1 first, T2 second)
{
    if ((first == pair.first && second == pair.second)
        || (first == pair.second && second == pair.first))
    {
        return true;
    }
    return false;
}


template<typename T>
concept ConvertibleToString = requires(T t) { std::to_string(t); };


template<ConvertibleToString T>
std::string to_string_with_precision(T t, std::streamsize prec, char decpoint)
{
    std::stringstream strstrm {};
    strstrm.precision(prec);
    
    strstrm.imbue(std::locale {strstrm.getloc(), new SetDecimalPoint {decpoint}});

    strstrm << t;
    
    return {strstrm.str()};
}
// END OF FUNCTIONS


// MACROS
#undef SINGLETON
#define SINGLETON(class_name)                 \
private:                                      \
    class_name () {}                          \
    ~class_name () {}                         \
    class_name (const  class_name&);          \
    class_name& operator=(const class_name&); \
public:                                       \
    static class_name& instance()             \
    {                                         \
        static class_name singleton;          \
        return singleton;                     \
    }                                         \
private:
// END OF MACROS

#endif  // GLOBAL_HPP
