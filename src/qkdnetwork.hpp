#ifndef QKDNETWORK_HPP 
#define QKDNETWORK_HPP 

#include <string>
#include <stdexcept>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp"
#include "qkdlink.hpp"
#include "qkdnode.hpp"
#include "qkdtopology.hpp"
//#include "qkdpathfinder.hpp"
#include "qkdrequestgen.hpp"
//#include "qkdkeygenmodel.hpp"

template < typename Clock = dclr::SysClock >
class QKD_Network
{
    using TimePoint = typename std::chrono::time_point< Clock >;
    
    private:

        QKD_Topology    < QKD_Network > mTopology;
        //QKD_Pathfinder  < QKD_Network >& mPathfinder;
        QKD_RequestGen  < QKD_Network > mRequestGen;
        //QKD_KeyGenModel < QKD_Network >& mKeyGenModel;

        std::map< NodeId, std::shared_ptr<QKD_Node> > mmNodeToId;
        std::map< LinkId, std::shared_ptr<QKD_Link> > mmLinkToId;

        std::vector< NodeId > maRemovedNodeIdList;
        std::vector< LinkId > maRemovedLinkIdList;

    public:
        
        QKD_Network ();

        NodeId addNode( std::string lbl = std::string 
            {"Node" + std::to_string( QKD_Node::last_node_id.value )} );
        LinkId addLink( QKD_Node&, QKD_Node&, dclr::Metrics m = 0 );

        void removeNode( NodeId );
        void removeLink( LinkId );

        QKD_Node& getNodeByVertex( Vertex& );
        QKD_Link& getLinkByEdge( Edge& );
        
        TimePoint now();
};

template < typename Clock >
QKD_Network< Clock >::QKD_Network ()
:
    mTopology { *this },
    mRequestGen { *this }
{}

template < typename Clock >
NodeId QKD_Network< Clock >::addNode( std::string lbl )
{
    VertexId v = mTopology.addVertex();
    QKD_Node* pn = new QKD_Node { mTopology.at( v ), lbl };
    mmNodeToId.emplace( pn->id, pn );
    return pn->id;
}

template < typename Clock >
LinkId QKD_Network< Clock >::addLink
    ( QKD_Node& n1, QKD_Node& n2, dclr::Metrics m ) 
{
    EdgeId e = mTopology.addEdge( n1.getVertex(), n2.getVertex() );
    QKD_Link* pl = new QKD_Link { mTopology.at( e ), m };
    mmLinkToId.emplace( pl->id, pl );
    return pl->id;
}

template < typename Clock >
void QKD_Network< Clock >::removeNode( NodeId n )
{
    for ( auto p = mmNodeToId.begin(); p != mmNodeToId.end(); ++ p )
        if ( p->first == n )
        {
            maRemovedNodeIdList.push_back( p->first );
            mmNodeToId.erase( p );
            break;
        }
}

template < typename Clock >
void QKD_Network< Clock >::removeLink( LinkId l )
{
    for ( auto p = mmLinkToId.begin(); p != mmLinkToId.end(); ++p )
        if ( p->first == l ) 
        {
            maRemovedLinkIdList.push_back( p->first );
            mmLinkToId.erase( p );
            break;
        }
}

template < typename Clock >
QKD_Node& QKD_Network< Clock >::getNodeByVertex( Vertex& v )
{
    for ( const auto& x : mmNodeToId )
        if ( *( (x.second)->mpVertex ) == v )
            return *x.second;
    throw std::out_of_range( "No node found for given vertex" );
}

template < typename Clock >
QKD_Link& QKD_Network< Clock >::getLinkByEdge( Edge& e )
{
    for ( const auto& x : mmLinkToId )
        if ( *( (x.second)->mpEdge ) == e )
            return *x.second;
    throw std::out_of_range( "No link found for given edge" );
}

template < typename Clock >
typename QKD_Network< Clock >::TimePoint 
QKD_Network< Clock >::now()
{
    return Clock::now();
}

#endif  // QKDNETWORK_HPP
