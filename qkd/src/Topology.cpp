#include "Topology.hpp"


#define _CHECK_DESC    "Topology: descriptor deleted: "
#define DELETE_NODE    "Topology::delete_node(): incorrect node descriptor: "
#define DELETE_LINK    "Topology::delete_link(): incorrect link descriptor: "
#define GET_ADJ_NODES  "Topology::get_adj_nodes(): link has only 1 or none adjacent nodes: "
#define GET_OTHER_NODE "Topology::get_other_node(): link isn't connected to node: "


// PRIVATE FUNCTIONS
void Topology::_check_descriptor(Descriptor desc) const
{
    if (PROP_TABLE(desc) == DescType::NOT_A_DESC)
        throw std::out_of_range {_CHECK_DESC + std::to_string(desc)};
}


std::vector<Descriptor> Topology::_descs_by_type(DescType dtype) const
{
    std::vector<Descriptor> descs {};
    for (size_t i = 0; i != PROP_TABLE.size_with_deleted().size_desc; ++i)
    {
        auto desc = static_cast<Descriptor>(i);
        DescType desc_type = prop_table()(desc);
        if (desc_type == dtype)
            descs.push_back(desc);
    }
    return descs;
}


std::vector<Descriptor> Topology::_adjacent(Descriptor desc) const
{
    std::vector<Descriptor> adj_descs;
    for (const auto& adj : _graph->adjacent(desc))
    {
        if (PROP_TABLE(adj) != DescType::NOT_A_DESC)
            adj_descs.push_back(adj);
    }
    std::sort(adj_descs.begin(), adj_descs.end());

    return adj_descs;
}


/*
void Topology::_cascade_shutdown(Descriptor desc, DescStatus dstatus)
{
    auto active_nodes = _get_adjacent(desc, DescType::NODE, DescStatus::ACTIVE);
    auto active_links = _get_adjacent(desc, DescType::LINK, DescStatus::ACTIVE);
    std::copy(active_links.begin(), active_links.end(), std::back_inserter(active_nodes));
    for (const auto& desc : )
}
*/
// END OF PRIVATE FUNCTIONS


// PUBLIC FUNCTIONS
Descriptor Topology::add_node(std::initializer_list<std::pair<std::string, std::string>> list)
{
    // add barrier
    Descriptor node_ptable = prop_table().add_entry(DescType::NODE);
    Descriptor node_graph = _graph->add_node(); 
    assert(node_ptable == node_graph);

    for (const auto& elem : list)
    {
        PROP_TABLE.add_property(elem.first);
        PROP_TABLE(node_ptable, elem.first) = elem.second;
    }
    BOOST_LOG_TRIVIAL(info) << "Topology: added node: " << node_ptable;

    return node_ptable;
}


void Topology::delete_node(Descriptor node)
{
    if (PROP_TABLE.entry_type(node) == DescType::NODE)
    {
        PROP_TABLE.delete_entry(node);
        // set nodes connected to this links inactive

        BOOST_LOG_TRIVIAL(info) << "Topology: deleted node: " << node;
        return;
    }
    throw std::logic_error {DELETE_NODE + std::to_string(node)};
}


Descriptor Topology::add_link(Descriptor node1, Descriptor node2,
                              std::initializer_list<std::pair<std::string, std::string>> list)
{
    Descriptor link_ptable = PROP_TABLE.add_entry(DescType::LINK);
    Descriptor link_graph = _graph->add_link(node1, node2);
    assert(link_ptable == link_graph);

    for (const auto& elem : list)
    {
        PROP_TABLE.add_property(elem.first);
        PROP_TABLE(link_ptable, elem.first) = elem.second;
    }
    BOOST_LOG_TRIVIAL(info) << "Topology: added link: " << link_ptable;

    return link_ptable;
}


void Topology::delete_link(Descriptor link)
{
    if (PROP_TABLE.entry_type(link) == DescType::LINK)
    {
        PROP_TABLE.delete_entry(link);
        // set nodes connected to this links inactive 

        BOOST_LOG_TRIVIAL(info) << "Topology: deleted link: " << link;
        return;
    }
    throw std::logic_error {DELETE_LINK + std::to_string(link)};
}


std::vector<Descriptor> Topology::nodes() const
{
    return _descs_by_type(DescType::NODE);
}


std::vector<Descriptor> Topology::links() const
{
    return _descs_by_type(DescType::LINK);
}


std::pair<Descriptor, Descriptor> Topology::adj_nodes(Descriptor link) const
{
    _check_descriptor(link);

    std::vector<Descriptor> adj_nodes = _adjacent(link);
    if (adj_nodes.size() < 2)
        throw std::out_of_range {GET_ADJ_NODES + std::to_string(link)};
    
    if (adj_nodes[0] > adj_nodes[1])
        std::swap(adj_nodes[0], adj_nodes[1]);

    return {adj_nodes[0], adj_nodes[1]};
}


std::vector<Descriptor> Topology::adj_links(Descriptor node) const
{
    _check_descriptor(node);
    
    return _adjacent(node);
}


Descriptor Topology::other_node(Descriptor link, Descriptor node) const
{
    _check_descriptor(link);
    _check_descriptor(node);

    auto nodes_adj = adj_nodes(link);

    if (nodes_adj.first == node)
        return nodes_adj.second;
    else if (nodes_adj.second == node)
        return nodes_adj.first;
    
    throw std::out_of_range {GET_OTHER_NODE + std::to_string(node)};
}


Descriptor Topology::adj_link(Descriptor node1, Descriptor node2) const
{
    _check_descriptor(node1);
    _check_descriptor(node2);

    Descriptor link = _graph->between(node1, node2);
    _check_descriptor(link);

    return link;
}
// END OF PUBLIC FUNCTIONS
