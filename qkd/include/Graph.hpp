#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Descriptor.hpp"


class Graph
{ 
public:
    virtual ~Graph() {}
    
    virtual bool is_node(Descriptor) const = 0;
    virtual bool is_link(Descriptor) const = 0;
    
    virtual Descriptor add_node() = 0;
    virtual Descriptor add_link(Descriptor, Descriptor) = 0;
    
    virtual std::vector<Descriptor> adjacent(Descriptor) const = 0;
    virtual std::vector<Descriptor> operator[](Descriptor) const = 0;    

    virtual Descriptor between(Descriptor, Descriptor) const = 0;
    virtual Descriptor operator()(Descriptor, Descriptor) const = 0;
};

#endif  // GRAPH_HPP
