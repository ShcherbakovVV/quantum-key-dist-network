#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/random/beta_distribution.hpp>

#include "AdjacencyMatrix.hpp"
#include "DijkstraShortestPath.hpp"
#include "KeyGenerationModel.hpp"
#include "Network.hpp"
#include "Pathfinder.hpp"
#include "QueueManager.hpp"
#include "Request.hpp"
#include "RequestGenerator.hpp"
#include "RequestQueue.hpp"
#include "Topology.hpp"
#include "FIFO.hpp"
#include "LIFO.hpp"
#include "PriorityQueue.hpp"

namespace br = boost::random;

int main()
{
    int exception_count = 0;

    Network network {};

    auto key_gen = std::make_shared<RNG<br::beta_distribution<KeyRate>>>();
    auto kgenmodel = std::make_shared<KeyGenerationModel>(key_gen);

    auto algorithm = std::make_shared<DijkstraShortestPath>();
    auto metrics_policy = std::make_shared<KeyAmountMetricsPolicy>();
    auto pathfinder = std::make_shared<Pathfinder>(algorithm, metrics_policy);

    auto reqqueue = std::make_shared<LimitedTimedFIFO>(10u);
    auto qmanager = std::make_shared<QueueManager>(reqqueue);

    auto reqbuilder = std::make_shared<TimedRequestBuilder>(10000);
    auto reqgenerator = std::make_shared<RequestGenerator>(reqbuilder);

    auto adj_matrix = std::make_shared<AdjacencyMatrix>();
    auto topology = std::make_shared<Topology>(adj_matrix);


    network.connect_module(ModuleType::KEY_GEN_MODEL, kgenmodel);
    network.connect_module(ModuleType::PATHFINDER, pathfinder);
    network.connect_module(ModuleType::QUEUE_MANAGER, qmanager);
    network.connect_module(ModuleType::REQ_GENERATOR, reqgenerator);
    network.connect_module(ModuleType::TOPOLOGY, topology);
    

    auto n0 = topology->add_node();
    auto n1 = topology->add_node();
    auto l2 = topology->add_link(n0, n1);
    auto n3 = topology->add_node();
    auto l4 = topology->add_link(n1, n3);
    auto n5 = topology->add_node();
    auto l6 = topology->add_link(n3, n5);
    auto n7 = topology->add_node();
    auto l8 = topology->add_link(n3, n7);
    Descriptor nonexistent_node = 9;
    Descriptor nonexistent_link = 10;


    assert((topology->nodes() == std::vector<Descriptor> {0, 1, 3, 5, 7}));
    assert((topology->links() == std::vector<Descriptor> {2, 4, 6, 8}));


    assert((pair_contains(topology->adj_nodes(l2), n0, n1)));
    assert((pair_contains(topology->adj_nodes(l4), n1, n3)));
    assert((pair_contains(topology->adj_nodes(l6), n3, n5)));
    assert((pair_contains(topology->adj_nodes(l8), n3, n7)));
    try {
        topology->adj_nodes(nonexistent_link);
    } catch (...) {
        ++exception_count;  // 1
    }


    assert((topology->adj_links(n0) == std::vector<Descriptor> {l2}));
    assert((topology->adj_links(n1) == std::vector<Descriptor> {l2, l4}));
    assert((topology->adj_links(n3) == std::vector<Descriptor> {l4, l6, l8}));
    assert((topology->adj_links(n5) == std::vector<Descriptor> {l6}));
    assert((topology->adj_links(n7) == std::vector<Descriptor> {l8}));
    try {
        topology->adj_links(nonexistent_node);
    } catch (...) {
        ++exception_count;  // 2
    }


    assert((topology->other_node(l2, n0) == n1));
    assert((topology->other_node(l2, n1) == n0));
    assert((topology->other_node(l4, n1) == n3));
    assert((topology->other_node(l4, n3) == n1));
    assert((topology->other_node(l6, n3) == n5));
    assert((topology->other_node(l6, n5) == n3));
    assert((topology->other_node(l8, n3) == n7));
    assert((topology->other_node(l8, n7) == n3));
    try {
        topology->other_node(l2, n3);
    } catch (...) {
        ++exception_count;  // 3
    }
    try {
        topology->other_node(nonexistent_link, n1);
    } catch (...) {
        ++exception_count;  // 4
    }
    try {
        topology->other_node(l2, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 5
    }
    try {
        topology->other_node(nonexistent_link, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 6
    }


    assert((topology->adj_link(n0, n1) == l2));
    assert((topology->adj_link(n1, n0) == l2));
    assert((topology->adj_link(n1, n3) == l4));
    assert((topology->adj_link(n3, n1) == l4));
    assert((topology->adj_link(n3, n5) == l6));
    assert((topology->adj_link(n5, n3) == l6));
    assert((topology->adj_link(n3, n7) == l8));
    assert((topology->adj_link(n7, n3) == l8));
    try {
        topology->adj_link(nonexistent_node, n1);
    } catch (...) {
        ++exception_count;  // 7
    }
    try {
        topology->adj_link(n1, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 8
    }


    try {
        topology->delete_node(nonexistent_node);
    } catch (...) {
        ++exception_count;  // 9
    }
    try {
        topology->delete_link(nonexistent_link);
    } catch (...) {
        ++exception_count;  // 10
    }


    topology->delete_node(n5);
    try {
        topology->delete_node(n5);
    } catch (...) {
        ++exception_count;  // 11
    }
    assert((topology->nodes() == std::vector<Descriptor> {0, 1, 3, 7}));
    assert((topology->links() == std::vector<Descriptor> {2, 4, 6, 8}));
    try {
        topology->adj_links(n5);
    } catch (...) {
        ++exception_count;  // 12
    }
    try {
        topology->adj_nodes(l6);
    } catch (...) {
        ++exception_count;  // 13
    }
    try {
        topology->other_node(l6, n5);
    } catch (...) {
        ++exception_count;  // 14
    }
    try {
        topology->other_node(l6, n3);
    } catch (...) {
        ++exception_count;  // 15
    }


    topology->delete_link(l8);
    try {
        topology->delete_link(n5);
    } catch (...) {
        ++exception_count;  // 16
    }
    assert((topology->nodes() == std::vector<Descriptor> {0, 1, 3, 7}));
    assert((topology->links() == std::vector<Descriptor> {2, 4, 6}));
    assert((topology->adj_links(n7) == std::vector<Descriptor> {}));
    try {
        topology->adj_link(n3, n7);
    } catch (...) {
        ++exception_count;  // 17
    }
    try {
        topology->adj_link(n7, n3);
    } catch (...) {
        ++exception_count;  // 18
    }
    try {
        topology->adj_nodes(l8);
    } catch (...) {
        ++exception_count;  // 19
    }
    try {
        topology->other_node(l8, n3);
    } catch (...) {
        ++exception_count;  // 20
    }
    try {
        topology->other_node(l8, n7);
    } catch (...) {
        ++exception_count;  // 21
    }

    if (exception_count == 21)
    {
        std::cout << "PASSED\n";
        return EXIT_SUCCESS;
    }
    std::cout << "FAILED\n";
    return EXIT_FAILURE;
}
