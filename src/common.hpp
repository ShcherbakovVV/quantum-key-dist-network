#ifndef COMMON_HPP
#define COMMON_HPP

#include <limits>
#include <chrono>
#include <concepts>

namespace tools
{
    template< typename Obj, std::signed_integral Rep >
    struct IdType
    {
        using obj_type = Obj;
        using value_type = Rep;

        value_type value; 

        IdType ( value_type id ) : value {id} {};

        IdType& operator++ ()
        {
            ++ value;
            return *this;
        }

        IdType& operator-- ()
        {
            -- value;
            return *this;
        }

        template< typename Obj2, typename Obj2 >
        bool operator== ( const IdType< Obj2, Rep2 >& id )
        {
            return std::is_same< obj_type, 
                                 typename IdType< Obj2, Rep2 >::obj_type
                               >::value
                   && value == id.value;
        }
    };

    template< typename Obj, std::integral Rep >
    std::ostream& operator<< ( std::ostream& os, const IdType< Obj, Rep >& id )
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
    Metrics METRICS_INFINITY = std::numeric_limits< Metrics >::max();

}  // namespace dclr

#endif  // COMMON_HPP

