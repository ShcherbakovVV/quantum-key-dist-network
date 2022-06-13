#include "Network.hpp"

#define _CHECK_MODULE_TYPE "QKD_Network: no module of such type connected: "
#define CONNECT_MODULE1    "QKD_Network::connect_module(): module of such type is already connected: "
#define CONNECT_MODULE2    "QKD_Network::connect_module(): this module is already connected as other type"


// PROTECTED FUNCTIONS
void Network::_check_module_type(ModuleType mod_type) const
{
    if (!_modules.contains(mod_type))
        throw std::out_of_range {_CHECK_MODULE_TYPE + mod_type_to_str(mod_type)};
}


void Network::clock_tick(Duration step)
{
    _prev = _curr;
    _curr += step;

    Clock::_now = _curr;
}
// PROTECTED FUNCTIONS


// PUBLIC FUNCTIONS
Network::~Network() 
{
    for (auto& [mod_type, mod] : _modules)
        mod->access().connect_to_network(nullptr);
}


void Network::connect_module(ModuleType mod_type, const std::shared_ptr<Module>& mod)
{
    if (_modules.contains(mod_type))
        throw std::logic_error {CONNECT_MODULE1 + mod_type_to_str(mod_type)};

    auto find_module = [&mod](const auto& elem) -> bool {
        if (elem.second == mod)
            return true;

        return false;
    };
    if (std::find_if(_modules.begin(), _modules.end(), find_module) != _modules.end())
        throw std::logic_error {CONNECT_MODULE2};

    _modules.insert({mod_type, mod});

    mod->access().connect_to_network(this);
}

                             
void Network::disconnect_module(ModuleType mod_type)
{
    _check_module_type(mod_type);
    
    _modules[mod_type]->access().connect_to_network(nullptr);
    _modules.erase(mod_type);
}


Module& Network::get_module(ModuleType mod_type)
{
    _check_module_type(mod_type);

    return *_modules.at(mod_type);
}


const Module& Network::get_module(ModuleType mod_type) const
{
    _check_module_type(mod_type);

    return *_modules.at(mod_type);
}


Module& Network::operator[](ModuleType mod_type)
{
    return get_module(mod_type);
}


const Module& Network::operator[](ModuleType mod_type) const
{
    return get_module(mod_type);
}
// END OF PUBLIC FUNCTIONS