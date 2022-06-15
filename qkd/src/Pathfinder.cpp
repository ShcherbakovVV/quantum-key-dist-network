#include "KeyGenerationModel.hpp"
#include "Network.hpp"
#include "Pathfinder.hpp"


Metrics Pathfinder::link_metrics(Descriptor link) const
{
    PROP_TABLE(link);  // checking

    return _metrics_policy->metrics(link);
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
