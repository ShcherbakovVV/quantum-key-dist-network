#include <condition_variable>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <thread>

#include <gtkmm/application.h>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/random/beta_distribution.hpp>

#include "SimulationWindow.hpp"
#include "QKD_Network.hpp"

//namespace bl = boost::log;


int main(int argc, char* argv[])
{
    /*
    bl::core::get()->set_filter(bl::trivial::severity >= bl::trivial::info);

    auto arrival_lambda = 0.001;
    auto service_lambda = 0.001;
    auto lifetime = 10000;
    auto capacity = 10;
    auto step = 10u;
    auto sim_time = 1000000u;

    QKD_Network qkd_network {};
     
    RNG<std::exponential_distribution<double>> arrivals {arrival_lambda};
    RNG<std::exponential_distribution<double>> service {service_lambda};

    // key generation rate 0-30 kbps
    auto key_generation = std::make_shared<RNG<br::beta_distribution<Metrics>>>();
    auto key_gen_model = std::make_shared<KeyGenerationModel>(key_generation);

    auto key_policy = std::make_shared<KeyAmountMetricsPolicy>();
    auto algorithm = std::make_shared<DijkstraShortestPath>();
    auto pathfinder = std::make_shared<Pathfinder>(algorithm, key_policy);

    // timed priority requests with 10 s lifetime with priority ranged 1-3
    auto req_builder = std::make_shared<TimedPriorityRequestBuilder>(lifetime, 1, 3);
    auto req_generator = std::make_shared<RequestGenerator>(req_builder);

    // queue of size 10
    auto req_queue = std::make_shared<LimitedTimedPriorityQueue>(capacity);
    auto queue_manager = std::make_shared<QueueManager>(req_queue);

    auto adj_matrix = std::make_shared<AdjacencyMatrix>();
    auto topology = std::make_shared<Topology>(adj_matrix);

    qkd_network.connect_module(ModuleType::KEY_GEN_MODEL, key_gen_model);
    qkd_network.connect_module(ModuleType::PATHFINDER, pathfinder);
    qkd_network.connect_module(ModuleType::REQ_GENERATOR, req_generator);
    qkd_network.connect_module(ModuleType::QUEUE_MANAGER, queue_manager);
    qkd_network.connect_module(ModuleType::TOPOLOGY, topology);

    auto nevs_smol   = topology->add_node({{"type", "target"}, {"label", "Невская Ратуша - Смольный"}, {"x pos", "6"}, {"y pos", "10.5"}});   // 1
    auto ats_smol    = topology->add_node({{"type", "aux"},    {"label", "ОВ АТС Смольного"},          {"x pos", "6"}, {"y pos", "12"}});   // 1
    auto ov_ttk      = topology->add_node({{"type", "aux"},    {"label", "ОВ ТТК"},                    {"x pos", "6"}, {"y pos", "13.5"}});   // 1
    auto okt_zd      = topology->add_node({{"type", "target"}, {"label", "Октябрьская ЖД"},            {"x pos", "6"}, {"y pos", "15"}});  // 2
    auto ivc_rzd_spb = topology->add_node({{"type", "aux"},    {"label", "ИВЦ СпБ РЖД"},               {"x pos", "0"}, {"y pos", "15"}});  // 3
    auto tosno       = topology->add_node({{"type", "aux"},    {"label", "Тосно"},                     {"x pos", "1"}, {"y pos", "13.5"}});  // 4
    auto chudovo     = topology->add_node({{"type", "aux"},    {"label", "Чудово"},                    {"x pos", "2"}, {"y pos", "12"}});  // 5
    auto mvishera    = topology->add_node({{"type", "aux"},    {"label", "Малая Вишера"},              {"x pos", "3"}, {"y pos", "10.5"}});   // 6
    auto torbino     = topology->add_node({{"type", "aux"},    {"label", "Торбино"},                   {"x pos", "4"}, {"y pos", "9"}});   // 7
    auto uglovka     = topology->add_node({{"type", "aux"},    {"label", "Угловка"},                   {"x pos", "5"}, {"y pos", "7.5"}});   // 8
    auto bologoe     = topology->add_node({{"type", "target"}, {"label", "Бологое"},                   {"x pos", "6"}, {"y pos", "6"}});   // 9
    auto udomlya     = topology->add_node({{"type", "aux"},    {"label", "Удомля"},                    {"x pos", "10"}, {"y pos", "6"}});   // 10
    auto kalininsky  = topology->add_node({{"type", "target"}, {"label", "ЦОД <<Калининский>>"},       {"x pos", "14"}, {"y pos", "6"}});  // 11
    auto vvolochek   = topology->add_node({{"type", "aux"},    {"label", "Вышний Волочек"},            {"x pos", "7"}, {"y pos", "4.5"}});   // 12
    auto spirovo     = topology->add_node({{"type", "aux"},    {"label", "Спирово"},                   {"x pos", "8"}, {"y pos", "3"}});   // 13
    auto likhoslavl  = topology->add_node({{"type", "aux"},    {"label", "Лихославль"},                {"x pos", "9"}, {"y pos", "1.5"}});   // 14
    auto tver        = topology->add_node({{"type", "aux"},    {"label", "Тверь"},                     {"x pos", "10"}, {"y pos", "0"}});   // 15
    auto zavidovo    = topology->add_node({{"type", "aux"},    {"label", "Завидово"},                  {"x pos", "11"}, {"y pos", "-1.5"}});   // 16
    auto klin        = topology->add_node({{"type", "aux"},    {"label", "Клин"},                      {"x pos", "12"}, {"y pos", "-3"}});   // 16
    auto kryukovo    = topology->add_node({{"type", "aux"},    {"label", "Крюково"},                   {"x pos", "13"}, {"y pos", "-4.5"}});   // 17
    auto gvc_rzd     = topology->add_node({{"type", "aux"},    {"label", "ГВЦ РЖД"},                   {"x pos", "14"}, {"y pos", "-6"}});   // 18
    auto m9          = topology->add_node({{"type", "target"}, {"label", "ЦОД <<М10>>"},               {"x pos", "18"}, {"y pos", "-3"}});   // 19
    auto c_rzd       = topology->add_node({{"type", "target"}, {"label", "Ц РЖД"},                     {"x pos", "18"}, {"y pos", "-4.5"}});   // 20
    auto mivc_rzd    = topology->add_node({{"type", "target"}, {"label", "МИВЦ РЖД"},                  {"x pos", "18"}, {"y pos", "-6"}});   // 21

    topology->add_link(nevs_smol,   ats_smol,    {{"distance", "50"}});
    topology->add_link(ats_smol,    ov_ttk,      {{"distance", "50"}});
    topology->add_link(ov_ttk,      ivc_rzd_spb, {{"distance", "50"}});
    topology->add_link(okt_zd,      ivc_rzd_spb, {{"distance", "50"}});
    topology->add_link(ivc_rzd_spb, tosno,       {{"distance", "50"}});
    topology->add_link(tosno,       chudovo,     {{"distance", "50"}});
    topology->add_link(chudovo,     mvishera,    {{"distance", "50"}});
    topology->add_link(mvishera,    torbino,     {{"distance", "50"}});
    topology->add_link(torbino,     uglovka,     {{"distance", "50"}});
    topology->add_link(uglovka,     bologoe,     {{"distance", "50"}});
    topology->add_link(kalininsky,  udomlya,     {{"distance", "50"}});
    topology->add_link(udomlya,     bologoe,     {{"distance", "50"}});
    topology->add_link(bologoe,     vvolochek,   {{"distance", "50"}});
    topology->add_link(vvolochek,   spirovo,     {{"distance", "50"}});
    topology->add_link(spirovo,     likhoslavl,  {{"distance", "50"}});
    topology->add_link(likhoslavl,  tver,        {{"distance", "50"}});
    topology->add_link(tver,        zavidovo,    {{"distance", "50"}});
    topology->add_link(zavidovo,    klin,        {{"distance", "50"}});
    topology->add_link(klin,        kryukovo,    {{"distance", "50"}});
    topology->add_link(kryukovo,    gvc_rzd,     {{"distance", "50"}});
    topology->add_link(gvc_rzd,     m9,          {{"distance", "50"}});
    topology->add_link(gvc_rzd,     c_rzd,       {{"distance", "50"}});
    topology->add_link(gvc_rzd,     mivc_rzd,    {{"distance", "50"}});
    
    qkd_network.simulation(arrivals, service, step, sim_time);
    */

    auto app = Gtk::Application::create("org.gtkmm.QKDsim");
    return app->make_window_and_run<SimulationWindow>(argc, argv, "QKDsim", 400, 800);
}