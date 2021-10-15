#include "qkdpathfinder.hpp"

Path::Path ( NodeId st, NodeId ds )
:
    start { st }, 
    dest { ds }
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

Path::~Path ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed " << *this;
}

const std::vector<LinkId>& Path::getPathLinkIdList() const
{
    return maPathLinkIdList;
}

void Path::addLinkToPath( LinkId l )
{
    maPathLinkIdList.push_back( l );
}

std::ostream& operator<< ( std::ostream& os, const Path& path )
{
    os << "Path {n" << path.start;
    for ( const auto& l : path.maPathLinkIdList )
        os << " -> l" << l;
    return os << "n" << path.dest << "}";
}
