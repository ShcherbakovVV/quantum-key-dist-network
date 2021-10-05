#ifndef QKDNETWORK_HPP 
#define QKDNETWORK_HPP 

#include <string>
#include <stdexcept>

#include <boost/log/trivial.hpp>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp"
#include "qkdlink.hpp"
#include "qkdnode.hpp"
#include "qkdtopology.hpp"
#include "qkdpathfinder.hpp"
#include "qkdrequestgen.hpp"
#include "qkdkeygenmodel.hpp"

class QKD_Network
{   
    private:

        QKD_Topology   <QKD_Network> mTopology;
        QKD_Pathfinder <QKD_Network> mPathfinder;
        QKD_RequestGen <QKD_Network> mRequestGen;
        QKD_KeyGenModel<QKD_Network> mKeyGenModel;

        std::map<NodeId, std::shared_ptr<QKD_Node> > mmNodeToId;
        std::map<LinkId, std::shared_ptr<QKD_Link> > mmLinkToId;

        std::vector< NodeId > maRemovedNodeIdList;
        std::vector< LinkId > maRemovedLinkIdList;

    public:
        
        QKD_Network ();

        // модификация сети
        NodeId addNode( const std::string lbl = std::string 
            {"Node" + std::to_string( QKD_Node::getLastNodeId().value )} );
        LinkId addLink( const QKD_Node&, const QKD_Node&, dclr::Metrics m = 0 );

        void removeNode( NodeId );
        void removeLink( LinkId );
        
        bool isNodeRemoved( NodeId );
        bool isLinkRemoved( LinkId );
        
        // получение информации
        Edge& getEdgeById( EdgeId );
        Vertex& getVertexById( VertexId );
        
        std::shared_ptr<QKD_Node> getNodePtrById( NodeId );
        std::shared_ptr<QKD_Link> getLinkPtrById( LinkId );
        
        QKD_Node& getNodeById( NodeId );
        QKD_Link& getLinkById( LinkId );
        
        QKD_Node& getNodeByVertex( const Vertex& );
        QKD_Link& getLinkByEdge( const Edge& );
        
        QKD_Node& getNodeByVertexId( VertexId );
        QKD_Link& getLinkByEdgeId( EdgeId );
        
        symmetric_pair<NodeId> getAdjNodeIds( LinkId l );
        
        // для алгоритмов
        std::vector<VertexId> getTopologyData();
        std::vector<EdgeId> getAdjEdgeIds( VertexId );
            
        void simulate();
};

#endif  // QKDNETWORK_HPP
