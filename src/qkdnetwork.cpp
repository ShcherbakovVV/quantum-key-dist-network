#include "qkdnetwork.hpp"

QKD_Network::QKD_Network ()
:
    mTopology    { *this },
    mPathfinder  { *this },
    mRequestGen  { *this },
    mKeyGenModel { *this }
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_Network\n";
}

NodeId QKD_Network::addNode( const std::string& lbl )
{
    VertexId v = mTopology.addVertex();
    QKD_Node* pn = new QKD_Node { mTopology.getVertexPtrById( v ), lbl };
    NodeId n_id = pn->getNodeId();
    mmNodeToId.emplace( n_id, pn );
    mRequestGen.updMaxNodeId();
    BOOST_LOG_TRIVIAL(trace) << "Added Node " << n_id << '\n';
    return n_id;
}

LinkId 
QKD_Network::addLink( const QKD_Node& n1, const QKD_Node& n2, dclr::Metrics m ) 
{
    EdgeId e = mTopology.addEdge( n1.getVertex().getVertexId(), 
                                  n2.getVertex().getVertexId() );
    QKD_Link* pl = new QKD_Link { mTopology.getEdgePtrById( e ), m };
    LinkId l_id = pl->getLinkId();
    mmLinkToId.emplace( l_id, pl );
    mRequestGen.updMaxNodeId();
    BOOST_LOG_TRIVIAL(trace) << "Added Link " << l_id << '\n';
    return l_id;
}

void QKD_Network::removeNode( NodeId n )
{
    for ( const auto& [node_id, node_ptr] : mmNodeToId )
        if ( node_id == n )
        {
            maRemovedNodeIdList.push_back( node_id );
            mmNodeToId.erase( node_id );
            BOOST_LOG_TRIVIAL(trace) << "Removed Node " << n << '\n';
            break;
        }
}

void QKD_Network::removeLink( LinkId l )
{
    for ( const auto& [link_id, link_ptr] : mmLinkToId )
        if ( link_id == l )
        {
            maRemovedLinkIdList.push_back( link_id );
            mmLinkToId.erase( link_id );
            BOOST_LOG_TRIVIAL(trace) << "Removed Link " << l << '\n';
            break;
        }
}

bool QKD_Network::isNodeRemoved( NodeId n )
{
    const auto b = maRemovedNodeIdList.begin();
    const auto e = maRemovedNodeIdList.end();
    if ( std::find( b, e, n ) != e )
        return true;
    return false;
}

bool QKD_Network::isLinkRemoved( LinkId l )
{
    const auto b = maRemovedLinkIdList.begin();
    const auto e = maRemovedLinkIdList.end();
    if ( std::find( b, e, l ) != e )
        return true;
    return false;
}

Edge& QKD_Network::getEdgeById( EdgeId e )
{
    return mTopology.getEdgeById( e );
}

Vertex& QKD_Network::getVertexById( VertexId v )
{
    return mTopology.getVertexById( v );
}

std::shared_ptr<QKD_Node> QKD_Network::getNodePtrById( NodeId n )
{
    try {
        std::shared_ptr<QKD_Node> p = mmNodeToId.at( n );
        return p;
    } catch (...) {
        throw std::out_of_range( "No node for given id" );
        //return nullptr;
    }
}

std::shared_ptr<QKD_Link> QKD_Network::getLinkPtrById( LinkId l )
{
    try {
        std::shared_ptr<QKD_Link> p = mmLinkToId.at( l );
        return p;
    } catch (...) {
        throw std::out_of_range( "No link for given id" );
        //return nullptr;
    }
}

QKD_Node& QKD_Network::getNodeById( NodeId n )
{
    return *getNodePtrById( n );
}

QKD_Link& QKD_Network::getLinkById( LinkId l )
{
    return *getLinkPtrById( l );
}

QKD_Node& QKD_Network::getNodeByVertex( const Vertex& v )
{
    for ( const auto& [node_id, node_ptr] : mmNodeToId )
        if ( *(node_ptr->mpVertex) == v )
            return *node_ptr;
    throw std::out_of_range( "No node for given vertex" );
}

QKD_Link& QKD_Network::getLinkByEdge( const Edge& e )
{
    for ( const auto& [link_id, link_ptr] : mmLinkToId )
        if ( *(link_ptr->mpEdge) == e )
            return *link_ptr;
    throw std::out_of_range( "No link for given edge" );
}

QKD_Node& QKD_Network::getNodeByVertexId( VertexId v )
{
    return getNodeByVertex( mTopology.getVertexById( v ) );
}

QKD_Link& QKD_Network::getLinkByEdgeId( EdgeId e )
{
    return getLinkByEdge( mTopology.getEdgeById( e ) );
}

symmetric_pair<NodeId> QKD_Network::getAdjNodeIds( LinkId l )
{
    QKD_Link& link = getLinkById( l );
    Edge& edge = link.getEdge();
    symmetric_pair<VertexId> vertices = edge.getAdjVertexIds();
    QKD_Node& n1 = getNodeByVertexId( vertices.first );
    QKD_Node& n2 = getNodeByVertexId( vertices.second );
    return symmetric_pair { n1.getNodeId(), n2.getNodeId() };
}

std::vector<VertexId> QKD_Network::getTopologyData()
{
    return mTopology.getTopologyData();
}

std::vector<EdgeId> QKD_Network::getAdjEdgeIds( VertexId v )
{
    return mTopology.getAdjEdgeIds( v );
}

void QKD_Network::simulate()
{
    while (true)
    {
        const auto req = mRequestGen.genRequest();
        const auto path = mPathfinder.invokeAlgorithm( req.start, req.dest );
        std::cout << path << '\n';
    }
}
