#include <iostream>

#include "DijkstraShortestPath.hpp"


// PUBLIC FUNCTIONS
const Path 
DijkstraShortestPath::invoke(const Pathfinder& pathfinder, 
                             Descriptor start_node, 
                             Descriptor dest_node) 
const
{
    auto& net = pathfinder.network();
    const auto& topology = dynamic_cast<const Topology&>(net.get_module(ModuleType::TOPOLOGY));
    
    std::map<Descriptor, Metrics> unvisited {};
    for (const auto& node : topology.access().nodes())
        unvisited[node] = METRICS_INFINITY;
    unvisited[start_node] = 0.f;
    
    Descriptor current_node = start_node;
    std::vector<Descriptor> path_nodes      {current_node};
    std::vector<Descriptor> blacklist_nodes {};

    Path path {start_node, dest_node};
    while (!unvisited.empty())
    {
        int candidate_count = 0;
        for (const auto& link : topology.access().adj_links(current_node))  // link iteration
        {
            Metrics metrics = pathfinder.access().link_metrics(link);
            
            if (metrics < 1.f)
                continue;

            Metrics distance = 1 / metrics;
            Metrics current_distance = unvisited.at(current_node);
            Metrics assign_distance = current_distance + distance;

            Descriptor neighbor_node = topology.access().other_node(link, current_node);
            try {
                if (unvisited.at(neighbor_node) > assign_distance)
                    unvisited.at(neighbor_node) = assign_distance;
            } catch (std::out_of_range& oor) {
                continue; 
            }
            ++candidate_count;
        }
        unvisited.erase(current_node);
        if (candidate_count == 0  // no candidates - insufficient keys
            && _map_min_value_pair(unvisited).second == METRICS_INFINITY) 
        {
            BOOST_LOG_TRIVIAL(warning) << "DijkstraShortestPaths: no quantum keys or no connection";
            return path;
        }
        current_node = _map_min_value_pair(unvisited).first;
        path_nodes.push_back(current_node);

        if (current_node == dest_node) 
            break;
    }

    int links_between = 0;  // количество несвязных вершин между двумя связными в path_vertices
    for (auto i = path_nodes.rbegin(); i != path_nodes.rend()-1; ++i)
    {
        // выбрасываем из path_vertices ненужные вершины
        Descriptor link;
        try {
            link = topology.adj_link(*(i-links_between), *(i+1));
        } catch (std::out_of_range& oor) {
            ++links_between;
            continue;
        }
        links_between = 0;

        path.append_link_front(link);
        if (*(i + 1) != start_node)
            path.append_node_front(*(i + 1));
    }
    return path;
}
// END OF PUBLIC FUNCTIONS
