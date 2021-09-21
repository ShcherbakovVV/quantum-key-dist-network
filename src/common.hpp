#ifndef COMMON_HPP
#define COMMON_HPP

#include <limits>
#include <chrono>
#include <concepts>

namespace tools
{
    template< typename Obj, std::signed_integral Rep >
    struct Id
    {
        using obj_type = Obj;
        using value_type = Rep;

        value_type value; 

        Id () : value { 0 } {};
        Id ( value_type id ) : value { id } {};

        Id& operator++ ()
        {
            ++ value;
            return *this;
        }

        Id& operator-- ()
        {
            -- value;
            return *this;
        }

        // добавить про конвертируемость
        template< std::signed_integral Rep2 >
        bool operator== ( const Id< Obj, Rep2 >& id )
        {
            return value == id.value;
        }
    };

    template< typename Obj, std::integral Rep >
    std::ostream& operator<< ( std::ostream& os, const Id< Obj, Rep >& id )
    {
        return os << id.value;
    }

    struct PrivateInitializer
    {
        template< typename T, typename... Args >
        static T InvokePrivateConstructor( Args... args )
        {
            return T ( args... ); 
        }
    };
}  // namespace tools

namespace dclr 
{
    using IdRep = int;

    // часы
    using SysClock = std::chrono::system_clock;
    using Duration = std::chrono::milliseconds;

    // тип метрики
    template< std::floating_point Double >
        using MetricsType = Double;
    using Metrics = MetricsType< double >;
    static Metrics METRICS_INFINITY = std::numeric_limits< Metrics >::max();

}  // namespace dclr

#endif  // COMMON_HPP

