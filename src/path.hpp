#ifndef PATH_HPP
#define PATH_HPP

#include <vector>
#include <iostream>
#include <algorithm>

#include "qkdlink.hpp"
#include "qkdnode.hpp"

template <std::unsigned_integral IdRep, typename Metrics>
struct PathT
{
    template <std::unsigned_integral Rep>
    friend std::ostream& operator<< ( std::ostream& os,
                                      const PathT<Rep, Metrics>& path )
    {
        os << "Path {n" << path.start << " -> ";
        for ( const auto& l : path.maPathLinkIdList )
            os << "l" << l << " -> ";
        return os << "n" << path.dest << "}";
    }

    template <typename NetworkModel>
        friend class DijkstraSP;

    using NodeId = typename QKD_NodeT<IdRep>::id_type;
    using LinkId = typename QKD_LinkT<IdRep, Metrics>::id_type;

    NodeId start;
    NodeId dest;

    const std::vector<LinkId>& getPathLinkIdList() const
        { return maPathLinkIdList; }

    bool isLinkInPath( LinkId ) const;

private:

    std::vector<LinkId> maPathLinkIdList;

    PathT () = delete;
    PathT ( NodeId, NodeId );

    void addLinkIdToPathBack ( LinkId l ) { maPathLinkIdList.push_back( l ); };
    void addLinkIdToPathFront( LinkId l )
        { maPathLinkIdList.insert( maPathLinkIdList.begin(), l ); };
};

template <std::unsigned_integral IdRep, typename Metrics>
PathT<IdRep, Metrics>::PathT ( NodeId st, NodeId ds )
:
    start { st },
    dest  { ds }
{}

template <std::unsigned_integral IdRep, typename Metrics>
bool PathT<IdRep, Metrics>::isLinkInPath( LinkId l ) const
{
    if ( std::find( maPathLinkIdList.begin(), maPathLinkIdList.end(), l )
         != maPathLinkIdList.end() )
        return true;
    return false;
}

#endif  // PATH_HPP
