#ifndef MODULE_HPP
#define MODULE_HPP

#include <stdexcept>

#include "Global.hpp"
#include "PropertyTable.hpp"


enum class ModuleType : size_t
{
    MODULE,
    KEY_GEN_MODEL,
    PATHFINDER,
    QUEUE_MANAGER,
    REQ_GENERATOR,
    TOPOLOGY
};

std::string mod_type_to_str(ModuleType);

class Network;

class Module
{
    friend class Network;
    friend class Simulation;

protected:
    const ModuleType _mod_type;

    Network* _network = nullptr;

    virtual void connect_to_network(Network*);
    virtual void disconnect_from_network([[maybe_unused]] Network* net = nullptr);

public:
    Module (ModuleType mod_type = ModuleType::MODULE)
    :
        _mod_type {mod_type}
    {}

    virtual ~Module() {}

    virtual Module&       access()       { return *this; }
    virtual const Module& access() const { return *this; }

    Network&       network();
    const Network& network() const;

    PropertyTable&       prop_table()       { return PropertyTable::instance(); }
    const PropertyTable& prop_table() const { return PropertyTable::instance(); }
};

#endif  // MODULE_HPP
