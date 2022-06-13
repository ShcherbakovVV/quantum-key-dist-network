#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

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

    auto reqqueue = std::make_shared<LimitedTimedFIFO>(10u);
    auto qmanager = std::make_shared<QueueManager>(reqqueue);

    auto reqbuilder = std::make_shared<TimedRequestBuilder>(10000);
    auto reqgenerator = std::make_shared<RequestGenerator>(reqbuilder);

    auto adj_matrix = std::make_shared<AdjacencyMatrix>();
    auto topology = std::make_shared<Topology>(adj_matrix);


    network.connect_module(ModuleType::KEY_GEN_MODEL, kgenmodel);
    network.connect_module(ModuleType::QUEUE_MANAGER, qmanager);
    network.connect_module(ModuleType::REQ_GENERATOR, reqgenerator);
    network.connect_module(ModuleType::TOPOLOGY, topology);


    auto algorithm = std::make_shared<DijkstraShortestPath>();
    auto metrics_policy1 = std::make_shared<KeyRateMetricsPolicy>();
    auto pathfinder1 = std::make_shared<Pathfinder>(algorithm, metrics_policy1);
    network.connect_module(ModuleType::PATHFINDER, pathfinder1);

    auto n0 = topology->add_node();
    auto n1 = topology->add_node();
    auto l2 = topology->add_link(n0, n1);
    auto n3 = topology->add_node();
    auto l4 = topology->add_link(n1, n3);
    auto n5 = topology->add_node();
    auto l6 = topology->add_link(n3, n5);
    auto n7 = topology->add_node();
    auto l8 = topology->add_link(n3, n7);
    auto not_connected_node = topology->add_node();
    Descriptor nonexistent_node = 10;

    for (Descriptor i = 0; i != network.prop_table().size_with_deleted().size_desc; ++i)
    {
        network.prop_table()(i, "key amount") = "100";
        network.prop_table()(i, "key rate, bps") = "100";
    }  

    const auto path1 = pathfinder1->get_path(n0, n7);
    assert((path1.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3, l8, n7}));
    const auto path2 = pathfinder1->get_path(n0, n5);
    assert((path2.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3, l6, n5}));
    const auto path3 = pathfinder1->get_path(n0, n3);
    assert((path3.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3}));
    const auto path4 = pathfinder1->get_path(n0, n1);
    assert((path4.path_data() == std::vector<Descriptor> {n0, l2, n1}));
    const auto path5 = pathfinder1->get_path(n7, n3);
    assert((path5.path_data() == std::vector<Descriptor> {n7, l8, n3}));
    const auto path6 = pathfinder1->get_path(n5, n3);
    assert((path6.path_data() == std::vector<Descriptor> {n5, l6, n3}));
    const auto path7 = pathfinder1->get_path(n3, n0);
    assert((path7.path_data() == std::vector<Descriptor> {n3, l4, n1, l2, n0}));
    
    try {
        pathfinder1->get_path(n0, n0);
    } catch (...) {
        ++exception_count;  // 1
    }
    try {
        pathfinder1->get_path(nonexistent_node, n0);
    } catch (...) {
        ++exception_count;  // 2
    }
    try {
        pathfinder1->get_path(n3, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 3
    }
    try {
        pathfinder1->get_path(nonexistent_node, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 4
    }
    pathfinder1->get_path(n1, not_connected_node);
    try {
        pathfinder1->get_path(nonexistent_node, not_connected_node);
    } catch (...) {
        ++exception_count;  // 5
    }
    try {
        pathfinder1->get_path(not_connected_node, not_connected_node);
    } catch (...) {
        ++exception_count;  // 6
    }

    network.disconnect_module(ModuleType::PATHFINDER);

    auto metrics_policy2 = std::make_shared<KeyAmountMetricsPolicy>();
    auto pathfinder2 = std::make_shared<Pathfinder>(algorithm, metrics_policy2);
    network.connect_module(ModuleType::PATHFINDER, pathfinder2);

    const auto path11 = pathfinder2->get_path(n0, n7);
    assert((path11.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3, l8, n7}));
    const auto path12 = pathfinder2->get_path(n0, n5);
    assert((path12.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3, l6, n5}));
    const auto path13 = pathfinder2->get_path(n0, n3);
    assert((path13.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3}));
    const auto path14 = pathfinder2->get_path(n0, n1);
    assert((path14.path_data() == std::vector<Descriptor> {n0, l2, n1}));
    const auto path15 = pathfinder2->get_path(n7, n3);
    assert((path15.path_data() == std::vector<Descriptor> {n7, l8, n3}));
    const auto path16 = pathfinder2->get_path(n5, n3);
    assert((path16.path_data() == std::vector<Descriptor> {n5, l6, n3}));
    const auto path17 = pathfinder2->get_path(n3, n0);
    assert((path17.path_data() == std::vector<Descriptor> {n3, l4, n1, l2, n0}));

    try {
        pathfinder2->get_path(n0, n0);
    } catch (...) {
        ++exception_count;  // 7
    }
    try {
        pathfinder2->get_path(nonexistent_node, n0);
    } catch (...) {
        ++exception_count;  // 8
    }
    try {
        pathfinder2->get_path(n3, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 9
    }
    try {
        pathfinder2->get_path(nonexistent_node, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 10
    }
    pathfinder2->get_path(n1, not_connected_node);
    try {
        pathfinder2->get_path(nonexistent_node, not_connected_node);
    } catch (...) {
        ++exception_count;  // 11
    }
    try {
        pathfinder2->get_path(not_connected_node, not_connected_node);
    } catch (...) {
        ++exception_count;  // 12
    }

    network.disconnect_module(ModuleType::PATHFINDER);

    auto metrics_policy3 = std::make_shared<KeyRateMetricsPolicy>();
    auto pathfinder3 = std::make_shared<Pathfinder>(algorithm, metrics_policy3);
    network.connect_module(ModuleType::PATHFINDER, pathfinder3);


    const auto path21 = pathfinder3->get_path(n0, n7);
    assert((path21.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3, l8, n7}));
    const auto path22 = pathfinder3->get_path(n0, n5);
    assert((path22.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3, l6, n5}));
    const auto path23 = pathfinder3->get_path(n0, n3);
    assert((path23.path_data() == std::vector<Descriptor> {n0, l2, n1, l4, n3}));
    const auto path24 = pathfinder3->get_path(n0, n1);
    assert((path24.path_data() == std::vector<Descriptor> {n0, l2, n1}));
    const auto path25 = pathfinder3->get_path(n7, n3);
    assert((path25.path_data() == std::vector<Descriptor> {n7, l8, n3}));
    const auto path26 = pathfinder3->get_path(n5, n3);
    assert((path26.path_data() == std::vector<Descriptor> {n5, l6, n3}));
    const auto path27 = pathfinder3->get_path(n3, n0);
    assert((path27.path_data() == std::vector<Descriptor> {n3, l4, n1, l2, n0}));

    try {
        pathfinder3->get_path(n0, n0);
    } catch (...) {
        ++exception_count;  // 13
    }
    try {
        pathfinder3->get_path(nonexistent_node, n0);
    } catch (...) {
        ++exception_count;  // 14
    }
    try {
        pathfinder3->get_path(n3, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 15
    }
    try {
        pathfinder3->get_path(nonexistent_node, nonexistent_node);
    } catch (...) {
        ++exception_count;  // 16
    }
    pathfinder3->get_path(n1, not_connected_node);
    try {
        pathfinder3->get_path(nonexistent_node, not_connected_node);
    } catch (...) {
        ++exception_count;  // 17
    }
    try {
        pathfinder3->get_path(not_connected_node, not_connected_node);
    } catch (...) {
        ++exception_count;  // 18
    }

    return EXIT_SUCCESS;
}