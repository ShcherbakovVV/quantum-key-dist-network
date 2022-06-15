#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <memory>
#include <vector>

#include <boost/log/trivial.hpp>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "KeyGenerationModel.hpp"
#include "Module.hpp"
#include "Path.hpp"
#include "RoutingAlgorithm.hpp"


/* TODO:
  - user defined metrics calculation formula
 */


struct MetricsPolicy
{
    virtual ~MetricsPolicy () {}

    virtual Metrics metrics(Descriptor) const = 0;
};


struct KeyAmountMetricsPolicy : MetricsPolicy
{
    virtual Metrics metrics(Descriptor link) const
    {
        return PROP_TABLE.property_as<KeyAmount>(link, "key amount");
    }
};


class Pathfinder : public Module
{
private:
    const std::shared_ptr<RoutingAlgorithm> _algorithm;
    const std::shared_ptr<MetricsPolicy>    _metrics_policy;

public:
    Pathfinder (const std::shared_ptr<RoutingAlgorithm> alg, 
                const std::shared_ptr<MetricsPolicy> mpolicy)
    :
        Module {ModuleType::PATHFINDER},
        _algorithm {alg},
        _metrics_policy {mpolicy}
    {}
    
    virtual ~Pathfinder () {}

    virtual Pathfinder&       access()       override { return *this; }
    virtual const Pathfinder& access() const override { return *this; }

    virtual Metrics link_metrics(Descriptor) const;
    
    virtual const Path get_path(Descriptor, Descriptor) const final;
    virtual const Path operator()(Descriptor, Descriptor) const final;
};

#endif  // PATHFINDER_HPP
