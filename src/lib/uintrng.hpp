#ifndef UINTRNG_HPP
#define UINTRNG_HPP

#include <random>
#include <limits>
#include <utility>
#include <concepts>

//template <typename D>
//concept Distribuition = requires ( D dist,  ) { dist() };

template <std::unsigned_integral UInt,
          std::uniform_random_bit_generator Eng,
          typename Dist>
class UIntRandNumGen
{
public:

    using Engine  = Eng;
    using Distrib = Dist;

private:

    Engine  eng;
    Distrib dist;

    static constexpr UInt int_max = std::numeric_limits<UInt>::max();

    UInt min_out_value;
    UInt max_out_value;

public:

    UIntRandNumGen ( UInt, UInt, Engine e, Distrib d );

    UInt operator() ();  // не const, т.к. dist(eng) не const

    void min_out( UInt new_min ) { min_out_value = new_min; };
    UInt min_out() const { return min_out_value; };

    void max_out( UInt new_max ) { max_out_value = new_max; };
    UInt max_out() const { return max_out_value; };
};

template <std::unsigned_integral UInt,
          std::uniform_random_bit_generator Eng,
          typename Dist>
UIntRandNumGen<UInt, Eng, Dist>::UIntRandNumGen
( UInt min, UInt max, Eng e, Dist d )
:
    min_out_value { min },
    max_out_value { max },
    eng { e },
    dist { d }
{}

template <std::unsigned_integral UInt,
          std::uniform_random_bit_generator Eng,
          typename Dist>
UInt UIntRandNumGen<UInt, Eng, Dist>::operator() ()
{
    UInt res;
    do {
        // std::random_device генерирует unsigned int
        res = dist( eng );
        int mag = static_cast<unsigned>( std::log10( int_max ) );
        res /= pow( 10, mag-1 );
        if ( res <= max_out_value && res >= min_out_value )
            break;
    }
    while (true);
    return res;
}

#endif  // UINTRNG_HPP
