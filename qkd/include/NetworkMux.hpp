#ifndef NETWORK_MUX_HPP
#define NETWORK_MUX_HPP

#include <algorithm>
#include <concepts>

#include "Module.hpp"
#include "Network.hpp"


template<typename ModType>
    requires std::is_base_of_v<Module, ModType>
class NetworkMux : public Module, Network
{
protected:
    std::vector<Network*> _networks;
    std::shared_ptr<ModType> _module;

    size_t _selected;

    virtual void connect_to_network(Network*) override;
    virtual void disconnect_from_network(Network*) override;

public:
    void connect_module(const std::shared_ptr<ModType>& mod);
    void disconnect_module();

    ModType&       access()       { return *_module; }
    const ModType& access() const { return *_module; }

    void select_network(Network* );

    virtual Network& network() override;

};


#endif  // NETWORK_MUX_HPP 