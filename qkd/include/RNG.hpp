#ifndef RNG_HPP
#define RNG_HPP

#include <concepts>
#include <random>

#include "Global.hpp"


template<typename Distribution>
class RNG
{
private:
    mutable std::random_device _rng {};
    mutable Distribution _dist;

    using ResultType = typename Distribution::result_type;
    using ParamType = typename Distribution::param_type;

public:
    template<typename ...Args>
    explicit RNG (Args... args)
    :
        _dist {args...}
    {}

    RNG (const Distribution& dist)
    :
        _dist {dist}
    {}

    RNG (const RNG<Distribution>&) {}
    RNG (RNG<Distribution>&&) {}
    
    template<typename ...Args>
    void reset_params(Args... args) 
    {
        _dist = Distribution {args...};
    }
    
    ResultType operator()() const 
    {
        return _dist(_rng);
    }
};


template<std::integral Int>
using UniformIntRNG = RNG<std::uniform_int_distribution<Int>>;

template<std::floating_point Float>
using NormalRealRNG = RNG<std::normal_distribution<Float>>;

#endif  // RNG_HPP
