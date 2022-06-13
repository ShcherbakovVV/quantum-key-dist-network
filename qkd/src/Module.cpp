#include "Module.hpp"

#define GET_NETWORK "Module::network(): module isn't connected to any networks: "


std::string mod_type_to_str(ModuleType mod_type)
{
    static std::string mod_types[] = 
    {
        "Module",
        "KeyGenerationModel",
        "Pathfinder",
        "QueueManager",
        "RequestGenerator",
        "Topology"
    };
    return mod_types[static_cast<unsigned>(mod_type)];
}


// PROTECTED FUNCTIONS
void Module::connect_to_network(Network* net)
{
    _network = net;
}


void Module::disconnect_from_network([[maybe_unused]] Network* net)
{
    _network = nullptr;
}
// END OF PROTECTED FUNCTIONS


// PUBLIC FUNCTIONS
Network& Module::network() 
{
    return const_cast<Network&>(const_cast<const Module*>(this)->network());
}


const Network& Module::network() const 
{
    if (!_network)
        throw std::logic_error {GET_NETWORK + mod_type_to_str(_mod_type)};
    
    return *_network;
}
// END OF PUBLIC FUNCTIONS

