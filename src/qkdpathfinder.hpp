#ifndef QKDPATHFINDER_HPP
#define QKDPATHFINDER_HPP

#include <map>
#include <vector>

#include <boost/log/trivial.hpp>

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
    
    ~Path ();
    
    const std::vector<LinkId>& getPathLinkIdList() const;
        
    private:

        std::vector<LinkId> maPathLinkIdList;

        Path () = delete;
        explicit Path ( NodeId, NodeId );
        
        void addLinkToPath( LinkId );
};

template <typename NetworkModel>
struct DijkstraShortestPaths
{
    Path operator() (const QKD_Node&, const QKD_Node&, const NetworkModel&);
};

template <typename NetworkModel>
Path 
DijkstraShortestPaths<NetworkModel>::operator() 
    ( const QKD_Node& st, const QKD_Node& ds, const NetworkModel& net)
{
    BOOST_LOG_TRIVIAL(trace) << "Invoked DijkstraShortestPaths";
    
    VertexId start_id = st.getVertexId();
    VertexId dest_id  = st.getVertexId();
    
    std::map<VertexId, dclr::Metrics> unvisited {};
    for ( const auto& x : net.getTopologyData() )
        unvisited[x] = dclr::METRICS_INFINITY;
    unvisited.at( start_id ) = 0;
    
    Path path { st.getNodeId(), ds.getNodeId() };
    while (true)
    {
        // итерации по смежным ребрам
        for ( const auto& e : net.getAdjEdgeIds( start_id ) )
        {
            VertexId neighbor = net.getEdgeById(e).getOtherVertexId(start_id);
            dclr::Metrics distance = net.getLinkByEdgeId(e).getMetricsValue();
            dclr::Metrics current_distance = unvisited.at( start_id );
            dclr::Metrics assign_distance = current_distance + distance;
            if ( unvisited.at( neighbor ) > assign_distance )
            {
                unvisited.at( neighbor ) = assign_distance;
                path.addLinkToPath( net.getLinkByEdgeId(e).getLinkId() );
            }
        }
        unvisited.erase( start_id );
        start_id = key_of_min_value( unvisited );
        if ( !unvisited.contains( dest_id )    // добрались до нужной вершины
             || unvisited.at( start_id ) == dclr::METRICS_INFINITY )  
                                               // граф несвязный
             break;
    }
    return path;
}

template <typename NetworkModel,
          typename Algorithm = DijkstraShortestPaths<NetworkModel>>
class QKD_Pathfinder
{
    private:
    
        NetworkModel& mQKD_Network;

    public:
    
        QKD_Pathfinder () = delete;
        QKD_Pathfinder ( NetworkModel& );
        
        ~QKD_Pathfinder ();
        
        Path invokeAlgorithm( NodeId, NodeId ) const;
};

template <typename NetworkModel, typename Algorithm>
QKD_Pathfinder<NetworkModel, Algorithm>::QKD_Pathfinder (NetworkModel& parent)
:
    mQKD_Network { parent }
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_Pathfinder";
}

template <typename NetworkModel, typename Algorithm>
QKD_Pathfinder<NetworkModel, Algorithm>::~QKD_Pathfinder ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_Pathfinder";
}

template <typename NetworkModel, typename Algorithm>
Path
QKD_Pathfinder<NetworkModel, Algorithm>::invokeAlgorithm(NodeId st, NodeId ds)
const
{
    Algorithm alg {};
    return alg( mQKD_Network.getNodeById( st ), 
                mQKD_Network.getNodeById( ds ),
                mQKD_Network );
}

#endif  // QKDPATHFINDER_HPP
