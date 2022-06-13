#include <cstdlib>
#include <iostream>
#include <memory>

#include "Module.hpp"
#include "Network.hpp"

int main()
{
    int exception_count = 0;
    
    auto mod1 = std::make_shared<Module>();
    auto mod2 = std::make_shared<Module>();
    auto mod3 = std::make_shared<Module>();
    auto mod4 = std::make_shared<Module>();
    auto mod5 = std::make_shared<Module>();

    mod1->prop_table();
    mod2->prop_table();
    mod3->prop_table();
    mod4->prop_table();
    mod5->prop_table();

    Network net {};
    net.connect_module(ModuleType::KEY_GEN_MODEL, mod1);
    net.connect_module(ModuleType::PATHFINDER, mod2);
    net.connect_module(ModuleType::QUEUE_MANAGER, mod3);
    net.connect_module(ModuleType::REQ_GENERATOR, mod4);
    net.connect_module(ModuleType::TOPOLOGY, mod5);

    mod1->network();
    mod2->network();
    mod3->network();
    mod4->network();
    mod5->network();

    net.get_module(ModuleType::KEY_GEN_MODEL);
    net.get_module(ModuleType::PATHFINDER);
    net.get_module(ModuleType::QUEUE_MANAGER);
    net.get_module(ModuleType::REQ_GENERATOR);
    net.get_module(ModuleType::TOPOLOGY);
    net[ModuleType::KEY_GEN_MODEL];
    net[ModuleType::PATHFINDER];
    net[ModuleType::QUEUE_MANAGER];
    net[ModuleType::REQ_GENERATOR];
    net[ModuleType::TOPOLOGY];

    auto mod11 = std::make_shared<Module>();
    try {
        net.connect_module(ModuleType::KEY_GEN_MODEL, mod11);
    } catch (...) {
        ++exception_count;  // 1
    }

    net.disconnect_module(ModuleType::TOPOLOGY);
    try {
        net.connect_module(ModuleType::TOPOLOGY, mod1);
    } catch (...) {
        ++exception_count;  // 2
    }

    try {
        net.get_module(ModuleType::TOPOLOGY);
    } catch (...) {
        ++exception_count;  // 3
    }
    try {
        net[ModuleType::TOPOLOGY];
    } catch (...) {
        ++exception_count;  // 4
    }

    try {
        net.disconnect_module(ModuleType::TOPOLOGY);
    } catch (...) {
        ++exception_count;  // 5
    }

    auto modd = std::make_shared<Module>();
    {
        Network net2 {};
        net2.connect_module(ModuleType::KEY_GEN_MODEL, modd);
    }
    try {
        modd->network();
    } catch (...) {
        ++exception_count;  // 6
    }

    if (exception_count == 6)
    {
        std::cout << "PASSED\n";
        return EXIT_SUCCESS;
    }
    std::cout << "FAILED\n";
    return EXIT_FAILURE;
}