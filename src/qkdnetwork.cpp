#include "qkdnetwork.hpp"

// class QKD_Node
QKD_Node::QKD_Node ( std::shared_ptr< Vertex > pv )
:
    mpVertex { pv }
{}

Vertex& QKD_Node::getVertex()
{
    return *mpVertex;
}

// class QKD_Link
QKD_Link::QKD_Link ( std::shared_ptr< Edge > pe,
                     common::Metrics m = common::METRICS_INFINITY )
:
    mpEdge { pe },
    mMetrics { m }
{}

Edge& QKD_Link::getEdge()
{
    return *mpEdge; 
}

// class QKD_Network
QKD_Network::QKD_Network ( Clock& clock )
:
    mTopology { *this },
    mPathfinder { *this },
    mRequestGen { *this },
    mKeyGenModel { *this },
    mClock { clock }
{}

QKD_Node& QKD_Network::addNode()
{
    /*Vertex v =*/ mTopology.addVertex();  // вывод нам не нужен
    QKD_Node* pn = new QKD_Node { mTopology.maVertexList.back() };    
    return *pn;
}

QKD_Link& QKD_Network::addLink( QKD_Node& n1, QKD_Node& n2 )
{
    /*Edge e =*/ mTopology.addEdge( n1.getVertex(), n2.getVertex() );
    QKD_Link* pl = new QKD_Link { mTopology.maEdgeList.back() };    
    return *pl;
}

void QKD_Network::removeNode( QKD_Node& node )
{
    delete &node; 
}

void QKD_Network::removeLink( QKD_Link& link )
{
    delete &link; 
}

QKD_Node& QKD_Network::getNodeByVertex( Vertex& v )
{
    for ( const auto& x : maNodeList )
    {
        if ( *( (*x)->mpVertex ) == v )
            return *x
    }
    return // not found
}

QKD_Link& QKD_Network::getLinkByEdge( Edge& e )
{
    for ( const auto& x : maLinkList )
    {
        if ( *( (*x)->mpEdge ) == e )
            return *x
    }
    return // not found
}
