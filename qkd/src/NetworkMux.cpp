#include "NetworkMux.hpp"


// PROTECTED FUNCTIONS
void NetworkMux::connect_to_network(Network* net)
{
    if (std::find(_networks.begin(), _networks.end(), net) != _networks.end())
        throw std::out_of_range {"NetworkMux::connect_to_network(): network is already connected"};

    _networks.push_back(net);
}


void NetworkMux::disconnect_from_network(Network* net)
{
    auto iter = std::find(_networks.begin(), _networks.end(), net);
    if (iter == _networks.end())
        throw std::out_of_range {"NetworkMux::disconnect_from_network(): network is not connected"};

    _networks.erase(iter);
}
// END OF PROTECTED FUNCTIONS


// PUBLIC FUNCTIONS
void NetworkMux::connect_module(const std::shared_ptr<ModType>& mod) 
{
    _module = mod;
}


void NetworkMux::disconnect_module() 
{
    _module.reset(nullptr);
}


void NetworkMux::select_network(Network* net)
{
    for (size_t = 0; i != _networks.size(); ++i)
    {
        if (_networks[i] == net)
            _selected = i;
    }
}

Network& NetworkMux::network() override
{
    return _networks[_selected];
}
// END OF PUBLIC FUNCTIONS