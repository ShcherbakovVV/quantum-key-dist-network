#include <cassert>
#include <chrono>
#include <cstdlib>
#include <thread>

#include <boost/random/beta_distribution.hpp>

#include "AdjacencyMatrix.hpp"
#include "Global.hpp"
#include "KeyGenerationModel.hpp"
#include "Network.hpp"
#include "Path.hpp"
#include "RNG.hpp"
#include "Topology.hpp"

namespace br = boost::random;


class NetworkTest : public Network
{
public:
    void tick(Duration dur)
    {
        clock_tick(dur);
    }
};


int main()
{
    NetworkTest network {};

    auto key_gen = std::make_shared<RNG<br::beta_distribution<KeyRate>>>();
    auto kgenmodel = std::make_shared<KeyGenerationModel>(key_gen);

    auto adj_matrix = std::make_shared<AdjacencyMatrix>();
    auto topology = std::make_shared<Topology>(adj_matrix);

    network.connect_module(ModuleType::KEY_GEN_MODEL, kgenmodel);
    network.connect_module(ModuleType::TOPOLOGY, topology);

    auto n0 = topology->access().add_node();
    auto n1 = topology->access().add_node();
    [[maybe_unused]] auto l2 = topology->access().add_link(n0, n1);
    auto n3 = topology->access().add_node();
    auto l4 = topology->access().add_link(n1, n3);
    auto n5 = topology->access().add_node();
    [[maybe_unused]] auto l6 = topology->access().add_link(n3, n5);
    auto n7 = topology->access().add_node();
    auto l8 = topology->access().add_link(n3, n7);

    for (size_t i = 0; i != 30; ++i)
    {
        network.tick(100);
        kgenmodel->generate_keys();
    }

    Path path {n1, n7};
    path.append_link_back(l4);
    path.append_node_back(n3);
    path.append_link_back(l8);
    assert((path));

    for (size_t i = 0; i != 30; ++i)
        kgenmodel->utilize_keys(path);

    return EXIT_SUCCESS;
}