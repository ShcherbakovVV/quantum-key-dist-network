#ifndef REQUEST_GENERATOR_HPP
#define REQUEST_GENERATOR_HPP

#include <chrono>
#include <concepts>
#include <iostream>
#include <memory>
#include <thread>
#include <random>

#include <boost/log/trivial.hpp>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "Module.hpp"
#include "Network.hpp"
#include "Request.hpp"
#include "RequestBuilder.hpp"
#include "RNG.hpp"
#include "Statistics.hpp"


class RequestGenerator : public Module
{
private:
    const std::shared_ptr<RequestBuilder> _req_builder;
    mutable UniformIntRNG<Descriptor> _node_chooser;

    bool _arrived = true;
    TimePoint _at;
    
    void _update_rng_params() const;
    Descriptor _random_node() const;
    
public:
    RequestGenerator (const std::shared_ptr<RequestBuilder>&);
    
    template<typename ArrivalDistrib>
    bool arrival(const ArrivalDistrib&);

    virtual RequestGenerator&       access()       override { return *this; }
    virtual const RequestGenerator& access() const override { return *this; }

    virtual const std::shared_ptr<Request> get_request() const;
};


template<typename ArrivalDistrib>
bool RequestGenerator::arrival(const ArrivalDistrib& arriv)
{
    if (_arrived)
    {
        TimePoint arriv_time;
        do {
            arriv_time = static_cast<TimePoint>(arriv());
        } while (arriv_time < network().step());

        _at = network().curr() + arriv_time;
        _arrived = false;
    }
    if (_at <= network().curr())
        _arrived = true;
        
    return _arrived;
}

#endif  // REQUEST_GENERATOR_HPP
