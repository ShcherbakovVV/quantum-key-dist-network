#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include <initializer_list>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/log/trivial.hpp>

#include "Descriptor.hpp"
#include "Graph.hpp"
#include "KeyGenerationModel.hpp"
#include "Module.hpp"
#include "Network.hpp"


class Topology : public Module
{
private:
    const std::shared_ptr<Graph> _graph;

    void _check_descriptor(Descriptor) const;
    std::vector<Descriptor> _descs_by_type(DescType) const;
    std::vector<Descriptor> _adjacent(Descriptor) const;
    
public:
    Topology (const std::shared_ptr<Graph>& graph)
    :
        Module {ModuleType::TOPOLOGY},
        _graph {graph}
    {}

    virtual Topology&       access()       override { return *this; }
    virtual const Topology& access() const override { return *this; }

    virtual Descriptor add_node(std::initializer_list<std::pair<std::string, std::string>> list = {});
    virtual void       delete_node(Descriptor);
    
    virtual Descriptor add_link(Descriptor, Descriptor, 
                                std::initializer_list<std::pair<std::string, std::string>> list = {});
    virtual void       delete_link(Descriptor);
    
    virtual std::vector<Descriptor> nodes() const;
    virtual std::vector<Descriptor> links() const;
    
    virtual std::pair<Descriptor, Descriptor> adj_nodes(Descriptor) const;
    virtual std::vector<Descriptor>           adj_links(Descriptor) const;
    
    virtual Descriptor other_node(Descriptor, Descriptor) const;
    virtual Descriptor adj_link(Descriptor, Descriptor) const;
};


#endif  // TOPOLOGY_HPP
