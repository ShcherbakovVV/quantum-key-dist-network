#ifndef QKDNETWORK_HPP 
#define QKDNETWORK_HPP 

#include "common.hpp"
#include "qkdtopology.hpp"
#include "qkdpathfinder.hpp"
#include "qkdrequestgen.hpp"
#include "qkdkeygenmodel.hpp"

class QKD_Node
{
    private:
        
        std::shared_ptr< Vertex > mpVertex; 

        QKD_Node () = delete; 
        QKD_Node ( std::shared_ptr< Vertex > pv ); 

    public:

        Vertex& getVertex();
}

class QKD_Link
{
    private:

        std::shared_ptr< Edge > mpEdge; 
        common::Metrics mMetrics;

        QKD_Link () = delete;
        QKD_Link ( std::shared_ptr< Edge > pe,
                   common::Metrics m = common::METRICS_INFINITY );

    public:

        Edge& getEdge();
}

template < using Clock = typename common::SysClock >
class QKD_Network
{
    private:

        QKD_Topology&    mTopology;
        QKD_Pathfinder&  mPathfinder;
        QKD_RequestGen&  mRequestGen;
        QKD_KeyGenModel& mKeyGenModel;

        Clock& mClock;

        std::vector< std::unique_ptr< QKD_Node > > maNodeList;
        std::vector< std::unique_ptr< QKD_Link > > maLinkList;

    public:
        /*
        QKD_Network (QKD_Topology& mTopology;
                     QKD_Pathfinder& mPathfinder;
                     QKD_RequestGen& mRequestGen;
                     QKD_KeyGenModel& mKeyGenModel;
        */
        QKD_Network ( Clock& clock );

        QKD_Node& addNode();
        QKD_Link& addLink();

        void removeNode( QKD_Node& node );
        void removeLink( QKD_Link& link );

        QKD_Node& getNodeByVertex( Vertex& v );
        QKD_Link& getLinkByEdge( Edge& e );
}

#endif  // QKDNETWORK_HPP
