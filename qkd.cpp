#include <chrono>
#include <random>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "src/qkdnetwork.hpp"

using SysClock     = typename std::chrono::system_clock;
using Duration     = typename std::chrono::milliseconds;
using RNG_Engine   = typename std::default_random_engine;
using Metrics      = double;
using IdRepr       = unsigned int;
using Distribution = std::uniform_int_distribution<IdRepr>;

using Network =
    QKD_Network<SysClock, Duration, RNG_Engine, Distribution, Metrics, IdRepr>;

int main()
{
    boost::log::core::get()->set_filter
        ( boost::log::trivial::severity >= boost::log::trivial::info );

    auto* p_top = new QKD_Topology<Network>    {};
    auto* p_pth = new QKD_Pathfinder<Network>  {};
    auto* p_rqg = new QKD_RequestGen<Network>  { 10000ms };
    auto* p_kgm = new QKD_KeyGenModel<Network> {};

    // const&?
    Network net { p_top, p_pth, p_rqg, p_kgm, 10 };

    auto n1 = net.addNode();
    auto n2 = net.addNode();
    auto n3 = net.addNode();
    auto n4 = net.addNode();
    auto n5 = net.addNode();

    [[maybe_unused]] auto l1 = net.addLink( n1, n2, 0 );
    [[maybe_unused]] auto l2 = net.addLink( n2, n3, 0 );
    [[maybe_unused]] auto l3 = net.addLink( n2, n4, 0 );

    std::thread kgen  = net.keyGenThread();
    std::thread rgen  = net.reqGenThread();
    std::thread rproc = net.reqProcThread<DijkstraSP<Network>>();
    kgen.join();
    rgen.join();
    rproc.join();

    return 0;
}
