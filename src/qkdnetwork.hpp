#ifndef QKDNETWORK_HPP 
#define QKDNETWORK_HPP 

#include <map>
#include <string>
#include <thread>
#include <stdexcept>

#include <boost/log/trivial.hpp>
#include <boost/circular_buffer.hpp>

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

        std::map<NodeId, std::shared_ptr<QKD_Node>> mmNodeToId;
        std::map<LinkId, std::shared_ptr<QKD_Link>> mmLinkToId;

        std::vector<NodeId> maRemovedNodeIdList;
        std::vector<LinkId> maRemovedLinkIdList;
        
        boost::circular_buffer<Request, std::allocator<Request>> maRequestQueue;
        std::mutex mLock;
        
        static std::string makeNodeLabel();
        
        /*        
        void reqGeneration();
        void keyGeneration();
        void reqProcessing();
        */

    public:
        
        QKD_Network ( size_t qcap = 10 );
        
        ~QKD_Network ();

        // модификация сети
        NodeId addNode( std::string lbl = makeNodeLabel() );  // строка&?
        LinkId addLink( NodeId, NodeId, dclr::Metrics m = 0);

        void removeNode( NodeId );
        void removeLink( LinkId );

        // получение информации
        bool isNodeRemoved( NodeId ) const;
        bool isLinkRemoved( LinkId ) const;
        
        Edge& getEdgeById( EdgeId ) const;
        Vertex& getVertexById( VertexId ) const;
        
        const std::shared_ptr<QKD_Node>& getNodePtrById( NodeId ) const;
        const std::shared_ptr<QKD_Link>& getLinkPtrById( LinkId ) const;
        
        QKD_Node& getNodeById( NodeId ) const;
        QKD_Link& getLinkById( LinkId ) const;
        
        QKD_Node& getNodeByVertex( const Vertex& ) const;
        QKD_Link& getLinkByEdge( const Edge& ) const;
        
        QKD_Node& getNodeByVertexId( VertexId ) const;
        QKD_Link& getLinkByEdgeId( EdgeId ) const;
        
        symmetric_pair<NodeId> getAdjNodeIds( LinkId l ) const;
        
        // буфер запросов
        void pushRequest();
        Request& popRequest();
        void processRequest( const Request& );
        
        // получение информации для алгоритмов
        std::vector<VertexId> getTopologyData() const;
        std::vector<EdgeId>   getAdjEdgeIds( VertexId ) const;
        
        void simulate();
        
        /*
        std::thread reqGenerationThr();
        std::thread keyGenerationThr();
        std::thread reqProcessingThr();
        */
};

#endif  // QKDNETWORK_HPP
