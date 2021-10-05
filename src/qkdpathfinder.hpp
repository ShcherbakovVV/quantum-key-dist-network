#ifndef QKDPATHFINDER_HPP
#define QKDPATHFINDER_HPP

#include <map>
#include <vector>

#include "common.hpp"
#include "qkdlink.hpp"
#include "qkdnode.hpp"

struct Path
{
    friend std::ostream& operator<< ( std::ostream&, const Path& );

    template <typename NetworkModel>
        friend class DijkstraShortestPaths;

    NodeId start;
    NodeId dest;
        
    private:

        std::vector<LinkId> maPathLinkIdList;

        Path () = delete;
        Path ( NodeId, NodeId );
        
        void addLinkToPath( LinkId );
};

Path::Path ( NodeId st, NodeId ds )
:
    start { st }, 
    dest { ds }
{}

void Path::addLinkToPath( LinkId l )
{
    maPathLinkIdList.push_back( l );
}

std::ostream& operator<< ( std::ostream& os, const Path& p )
{
    os << p.start << "--";
    for ( const auto& l : p.maPathLinkIdList )
        os << l << "--";
    return os << p.dest;
}

template< typename NetworkModel >
struct DijkstraShortestPaths
{
    const Path operator() ( QKD_Node&, QKD_Node&, NetworkModel& );
};

template< typename NetworkModel>
const Path 
DijkstraShortestPaths<NetworkModel>::operator() 
    ( QKD_Node& st, QKD_Node& ds, NetworkModel& net)
{
    VertexId start_id = st.getVertex().getVertexId();
    VertexId dest_id  = st.getVertex().getVertexId();
    
    std::map<VertexId, dclr::Metrics, std::less<dclr::Metrics>> unvisited {};
    for ( const auto& x : net.getTopologyData() )
        unvisited.insert( x, dclr::METRICS_INFINITY );
    unvisited.at( start_id ) = 0;
    
    Path path { st.getNodeId(), ds.getNodeId() };
    while (true)
    {
        // итерации по смежным ребрам
        std::vector<EdgeId> adj_edges = net.getAdjEdgeIds( start_id );
        for ( const auto& e : adj_edges )
        {
            VertexId neighbor = net.getEdgeById(e).getOtherVertexId(start_id);
            dclr::Metrics distance = net.getLinkByEdgeId(e).getMetricsValue();
            dclr::Metrics current_distance = unvisited.at( start_id );
            dclr::Metrics assign_distance = current_distance + distance;
            if ( unvisited.at( neighbor ) > assign_distance )
            {
                unvisited.at( neighbor ) = assign_distance;
                path.addLinkToPath( net.getLinkByEdgeId(e) );
            }
        }
        unvisited.erase( start_id );
        start_id = key_of_min_value( unvisited );
        if ( !unvisited.contains( dest_id )    // добрались до нужной вершины
             || unvisited.at(start_id) == dclr::METRICS_INFINITY )  
                                               // граф несвязный
             break;
    }
    return path;
}

template< typename NetworkModel,
          typename Algorithm = DijkstraShortestPaths >
class QKD_Pathfinder
{
    private:
    
        const NetworkModel& mQKD_Network;

    public:
    
        QKD_Pathfinder () = delete;
        QKD_Pathfinder ( NetworkModel& );
        
        const Path invokeAlgorithm( NodeId, NodeId );
};

typename< typename NetworkModel, typename Algorithm >
QKD_Pathfinder ( NetworkModel& parent )
:
    mQKD_Network { parent }
{}

template< typename NetworkModel, typename Algorithm >
const Path
QKD_Pathfinder<NetworkModel, Algorithm>::invokeAlgorithm(NodeId st, NodeId ds)
{
    Algorithm alg {};
    return alg( mQKD_Network.getNodeById( st ), 
                mQKD_Network.getNodeById( ds ),
                mQKD_Network );
}

#endif  // QKDPATHFINDER_HPP
