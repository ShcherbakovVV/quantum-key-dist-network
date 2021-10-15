#ifndef UINTRNG_HPP
#define UINTRNG_HPP

#include <random>
#include <concepts>

// сделать более общим
template <std::unsigned_integral UInt, template<typename> class Dist>
class UIntRandNumGen
{
    private:
       
        Dist<UInt> dist;
        std::random_device rd;
        
        UInt min_out_value;
        UInt max_out_value;
        
    public:
   
        UIntRandNumGen ( UInt, UInt, Dist<UInt>&& );
        
        UInt operator() ();  // не const, т.к. dist(rd) не const
    
        void min_out( UInt );
        UInt min_out() const;
        
        void max_out( UInt );
        UInt max_out() const;
};

template <std::unsigned_integral UInt, template<typename> class Dist>
UIntRandNumGen<UInt, Dist>::UIntRandNumGen 
( UInt min, UInt max, Dist<UInt>&& d )
:
    min_out_value { min },
    max_out_value { max },
    dist { d }
{}

template <std::unsigned_integral UInt, template<typename> class Dist>
UInt UIntRandNumGen<UInt, Dist>::operator() ()
{
    UInt res;
    do {
        // std::random_device генерирует unsigned int 
        res = dist(rd);
        int mag = 
            static_cast<int>(std::log10(std::numeric_limits<UInt>::max()));
        res /= pow( 10, mag - 1 );
        if ( res <= max_out_value && res >= min_out_value )
            break;
    }
    while (true);
    return res;
}

template <std::unsigned_integral UInt, template<typename> class Dist>
void UIntRandNumGen<UInt, Dist>::min_out( UInt new_min )
{
    min_out_value = new_min;
}

template <std::unsigned_integral UInt, template<typename> class Dist>
UInt UIntRandNumGen<UInt, Dist>::min_out() const
{
    return min_out_value;
}

template <std::unsigned_integral UInt, template<typename> class Dist>
void UIntRandNumGen<UInt, Dist>::max_out( UInt new_max )
{
    max_out_value = new_max;
}

template <std::unsigned_integral UInt, template<typename> class Dist>
UInt UIntRandNumGen<UInt, Dist>::max_out() const
{
    return max_out_value;
}

#endif  // UINTRNG_HPP
