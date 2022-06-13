#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "Global.hpp"
#include "RNG.hpp"


template<typename ArrivalDistrib, typename ServiceDistrib>
class Simulation
{
private:
    ArrivalDistrib _arrival;
    ServiceDistrib _service;

public:
    Simulation (ArrivalDistrib a, ServiceDistrib s, Duration step, ) 
    :
        _arrival {a},
        _service {s}
    {}

    void setting

    template<typename ...N>
    void start(N&... networks)
    {

    }
};


#endif  // SIMULATION_HPP