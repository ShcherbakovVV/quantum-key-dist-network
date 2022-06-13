#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>

#include "Global.hpp"
#include "Module.hpp"
#include "PropertyTable.hpp"


class Network
{
protected:
    std::map<ModuleType, std::shared_ptr<Module>> _modules;
    //std::mutex _mutex;
    TimePoint _prev = 0;
    TimePoint _curr = 0;
    Duration _step;

    void _check_module_type(ModuleType) const;

    virtual void clock_tick(Duration);

public:
    virtual ~Network();

    virtual void connect_module(ModuleType, const std::shared_ptr<Module>& mod);
    virtual void disconnect_module(ModuleType);

    PropertyTable&       prop_table()       { return PropertyTable::instance(); }
    const PropertyTable& prop_table() const { return PropertyTable::instance(); }

    virtual Module&       get_module(ModuleType);
    virtual const Module& get_module(ModuleType) const;

    virtual TimePoint prev() const { return _prev; }
    virtual TimePoint curr() const { return _curr; }
    virtual Duration step() const { return _step; }

    virtual Module&       operator[](ModuleType);
    virtual const Module& operator[](ModuleType) const;
};


#endif  // NETWORK_HPP
