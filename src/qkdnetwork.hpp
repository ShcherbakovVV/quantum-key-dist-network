#ifndef QKDNETWORK_HPP 
#define QKDNETWORK_HPP 

#include <string>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp" 
#include "qkdtopology.hpp"
//#include "qkdpathfinder.hpp"
#include "qkdrequestgen.hpp"
//#include "qkdkeygenmodel.hpp"

template < typename Clock = dclr::SysClock >
class QKD_Network
{
    private:

        QKD_Topology    < QKD_Network > mTopology;
        //QKD_Pathfinder  < QKD_Network >& mPathfinder;
        QKD_RequestGen  < QKD_Network > mRequestGen;
        //QKD_KeyGenModel < QKD_Network >& mKeyGenModel;

        Clock& mClock;

        std::map< NodeId, std::unique_ptr<QKD_Node> > mmNodeToId;
        std::map< LinkId, std::unique_ptr<QKD_Link> > mmLinkToId;

        std::vector< NodeId > maRemovedNodeIdList;
        std::vector< LinkId > maRemovedLinkIdList;

    public:

        QKD_Network ( Clock& );

        void initNetwork();

        NodeId addNode( std::string );
        LinkId addLink( QKD_Node&, QKD_Node&, dclr::Metrics );

        void removeNode( NodeId );
        void removeLink( LinkId );

        QKD_Node& getNodeByVertex( Vertex& );
        QKD_Link& getLinkByEdge( Edge& );
};

template < typename Clock = dclr::SysClock >
QKD_Network< Clock >::QKD_Network ( Clock& clock )
:
    mClock { clock }
{}

template < typename Clock = dclr::SysClock >
void QKD_Network< Clock >::initNetwork()
{
    using NetworkModel = decltype(*this);
    mTopology = QKD_Topology< NetworkModel > { *this };
    //mPathfinder { *this };
    mRequestGen = QKD_RequestGen< NetworkModel > { *this };
    //mKeyGenModel { *this };
}

template < typename Clock = dclr::SysClock >
NodeId QKD_Network< Clock >::addNode
    ( std::string lbl = std::string 
        {"Node" + std::to_string( QKD_Node::last_node_id ) } )
{
    /*VertexId v =*/ mTopology.addVertex();  // вывод нам не нужен
    QKD_Node* pn = new QKD_Node { mTopology.getNewestVertex(), lbl };
    mmNodeToId.emplace( pn->id, pn );
    return pn->id;
}

template < typename Clock = dclr::SysClock >
LinkId QKD_Network< Clock >::addLink
    ( QKD_Node& n1, QKD_Node& n2, dclr::Metrics m = 0 ) 
{
    /*EdgeId e =*/ mTopology.addEdge( n1.getVertex(), n2.getVertex() );
    QKD_Link* pl = new QKD_Link { mTopology.getNewestEdge(), m };
    mmLinkToId.emplace( pl );
    return pl->id;
}

template < typename Clock = dclr::SysClock >
void QKD_Network< Clock >::removeNode( NodeId n )
{
    ` 
    for ( auto p = mmNodeToId.begin(); p != mmNodeToId.end(); ++ p )
        if ( *(*p) == n )
        {
            maRemovedNodeIdList.push_back( (*p)->id );
            mmNodes.erase( p );
            break;
        }
}

template < typename Clock = dclr::SysClock >
void QKD_Network< Clock >::removeLink( QKD_Link& l )
{
    for ( auto p = maLinkList.begin(); p != maLinkList.end(); ++p )
    {
        if ( *(*p) == l ) 
        {
            maRemovedLinkIdList.push_back( (*p)->id );
            maLinkList.erase(p);
            break;
        }
    } 
}

template < typename Clock = dclr::SysClock >
QKD_Node& QKD_Network< Clock >::getNodeByVertex( Vertex& v )
{
    for ( const auto& x : mmNodes )
    {
        if ( *( x->mpVertex ) == v )
            return *x;
    }
    return QKD_NODE_INVALID;
}

template < typename Clock = dclr::SysClock >
QKD_Link& QKD_Network< Clock >::getLinkByEdge( Edge& e )
{
    for ( const auto& x : maLinkList )
    {
        if ( *( x->mpEdge ) == e )
            return *x;
    }
    return QKD_LINK_INVALID;
}

#endif  // QKDNETWORK_HPP
