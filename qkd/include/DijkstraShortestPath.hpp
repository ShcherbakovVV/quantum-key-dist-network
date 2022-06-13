#ifndef DIJKSTRA_SHORTEST_PATH_HPP
#define DIJKSTRA_SHORTEST_PATH_HPP

#include <algorithm>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/log/trivial.hpp>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "Network.hpp"
#include "Path.hpp"
#include "Pathfinder.hpp"
#include "RoutingAlgorithm.hpp"
#include "Topology.hpp"


class DijkstraShortestPath final : public RoutingAlgorithm
{
private:
    template<typename Key, typename Value>
    std::pair<Key, Value> _map_min_value_pair(const std::map<Key, Value>&) const;

public:
    const Path 
    invoke(const Pathfinder&, Descriptor, Descriptor) 
    const override final;

    const Path 
    operator()(const Pathfinder& p, Descriptor s, Descriptor d) 
    const override final
    {
        return invoke(p, s, d); 
    }
};


// PRIVATE FUNCTIONS
template<typename Key, typename Value>
std::pair<Key, Value> 
DijkstraShortestPath::_map_min_value_pair(const std::map<Key, Value>& map)
const
{
    Value min_key   = map.begin()->first;
    Value min_value = map.begin()->second;
    for (const auto& [key, value] : map)
    {
        if (min_value > value)
        {
            min_value = value;
            min_key = key;
        }
    }
    return {min_key, min_value};
}
// END OF PRIVATE FUNCTIONS


#endif  // DIJKSTRA_SHORTEST_PATH_HPP
