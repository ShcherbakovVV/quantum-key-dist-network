#include "KeyGenerationModel.hpp"
#include "Network.hpp"
#include "Pathfinder.hpp"
#include "PropertyTable.hpp"


Metrics Pathfinder::link_metrics(Descriptor link) const
{
    PROP_TABLE(link);  // checking

    auto key_data = dynamic_cast<const KeyGenerationModel&>
                        (network()[ModuleType::KEY_GEN_MODEL].access()).key_data(link);

    return _metrics_policy->metrics(key_data);
}


const Path Pathfinder::get_path(Descriptor start, Descriptor dest) const
{
    PROP_TABLE(start);  // checking
    PROP_TABLE(dest);

    auto path = _algorithm->invoke(*this, start, dest);
    if (!path)
    {
        BOOST_LOG_TRIVIAL(warning) << "Pathfinder: no path found";
        return path;
    }
    BOOST_LOG_TRIVIAL(info) << "Pathfinder: found path " << path;
    return path;
}


const Path Pathfinder::operator()(Descriptor start, Descriptor dest) const
{
    return get_path(start, dest);
}
