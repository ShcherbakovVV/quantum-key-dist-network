#ifndef ROUTING_ALGORITHM_HPP
#define ROUTING_ALGORITHM_HPP

#include "Descriptor.hpp"


class Path;
class Pathfinder;


struct RoutingAlgorithm
{
    virtual ~RoutingAlgorithm() {}
    
    virtual const Path invoke(const Pathfinder&, Descriptor, Descriptor) const = 0;
    virtual const Path operator()(const Pathfinder&, Descriptor, Descriptor) const = 0;
};

#endif  // ROUTING_ALGORITHM_HPP
