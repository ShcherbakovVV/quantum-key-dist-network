#include "qkdnetwork.hpp"

QKD_Network::QKD_Network ( size_t qcap )
:
    mTopology      { *this },
    mPathfinder    { *this },
    mRequestGen    { *this },
    mKeyGenModel   { *this },
    maRequestQueue { qcap }
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_Network";
}

QKD_Network::~QKD_Network ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_Network";
}

std::string QKD_Network::makeNodeLabel()
{
    std::string label = "Node";
    return label + std::to_string(QKD_Node::getLastNodeId().value);
}

// модификация сети
/*******************************************************************/
NodeId QKD_Network::addNode( std::string lbl )
{
    VertexId v = mTopology.addVertex();
    QKD_Node* pn = new QKD_Node { mTopology.getVertexPtrById( v ), lbl };
    NodeId n_id = pn->getNodeId();
    mmNodeToId.emplace( n_id, pn );
    mRequestGen.updGenParams();
    BOOST_LOG_TRIVIAL(trace) << "QKD_Network: Added " << *pn;
    return n_id;
}

LinkId QKD_Network::addLink( NodeId n1, NodeId n2, dclr::Metrics m) 
{
    EdgeId e = mTopology.addEdge( getNodeById(n1).getVertexId(), 
                                  getNodeById(n2).getVertexId() );
    QKD_Link* pl = new QKD_Link { mTopology.getEdgePtrById( e ), m };
    LinkId l_id = pl->getLinkId();
    mmLinkToId.emplace( l_id, pl );
    mRequestGen.updGenParams();
    BOOST_LOG_TRIVIAL(trace) << "QKD_Network: Added " << *pl;
    return l_id;
}

void QKD_Network::removeNode( NodeId n )
{
    for ( const auto& [node_id, node_ptr] : mmNodeToId )
        if ( node_id == n )
        {
            // должно стоять в конце блока
            BOOST_LOG_TRIVIAL(trace) << "QKD_Network: Removed " << *node_ptr;
            maRemovedNodeIdList.push_back( node_id );
            mmNodeToId.erase( node_id );
            break;
        }
}

void QKD_Network::removeLink( LinkId l )
{
    for ( const auto& [link_id, link_ptr] : mmLinkToId )
        if ( link_id == l )
        {
            BOOST_LOG_TRIVIAL(trace) << "QKD_Network: Removed " << *link_ptr;
            maRemovedLinkIdList.push_back( link_id );
            mmLinkToId.erase( link_id );
            break;
        }
}
/*******************************************************************/

// получение информации
/*******************************************************************/
bool QKD_Network::isNodeRemoved( NodeId n ) const
{
    const auto b = maRemovedNodeIdList.begin();
    const auto e = maRemovedNodeIdList.end();
    if ( std::find( b, e, n ) != e )
        return true;
    return false;
}

bool QKD_Network::isLinkRemoved( LinkId l ) const
{
    const auto b = maRemovedLinkIdList.begin();
    const auto e = maRemovedLinkIdList.end();
    if ( std::find( b, e, l ) != e )
        return true;
    return false;
}

Edge& QKD_Network::getEdgeById( EdgeId e ) const
{
    return mTopology.getEdgeById( e );
}

Vertex& QKD_Network::getVertexById( VertexId v ) const
{
    return mTopology.getVertexById( v );
}

const std::shared_ptr<QKD_Node>& QKD_Network::getNodePtrById( NodeId n ) const
{
    try {
        return mmNodeToId.at( n );
    } catch (...) {
        throw std::out_of_range( "No QKD_Node for given Id" );
    }
}

const std::shared_ptr<QKD_Link>& QKD_Network::getLinkPtrById( LinkId l ) const
{
    try {
        return mmLinkToId.at( l );
    } catch (...) {
        throw std::out_of_range( "No QKD_Link for given Id" );
    }
}

QKD_Node& QKD_Network::getNodeById( NodeId n ) const
{
    return *getNodePtrById( n );
}

QKD_Link& QKD_Network::getLinkById( LinkId l ) const
{
    return *getLinkPtrById( l );
}

QKD_Node& QKD_Network::getNodeByVertex( const Vertex& v ) const
{
    for ( const auto& [node_id, node_ptr] : mmNodeToId )
        if ( *(node_ptr->mpVertex) == v )
            return *node_ptr;
    throw std::out_of_range( "No QKD_Node for given Vertex&" );
}

QKD_Link& QKD_Network::getLinkByEdge( const Edge& e ) const
{
    for ( const auto& [link_id, link_ptr] : mmLinkToId )
        if ( *(link_ptr->mpEdge) == e )
            return *link_ptr;
    throw std::out_of_range( "No QKD_Link for given Edge&" );
}

QKD_Node& QKD_Network::getNodeByVertexId( VertexId v ) const
{
    return getNodeByVertex( mTopology.getVertexById( v ) );
}

QKD_Link& QKD_Network::getLinkByEdgeId( EdgeId e ) const
{
    return getLinkByEdge( mTopology.getEdgeById( e ) );
}

symmetric_pair<NodeId> QKD_Network::getAdjNodeIds( LinkId l ) const
{
    QKD_Link& link = getLinkById( l );
    Edge& edge = link.getEdge();
    symmetric_pair<VertexId> vertices = edge.getAdjVertexIds();
    QKD_Node& n1 = getNodeByVertexId( vertices.first );
    QKD_Node& n2 = getNodeByVertexId( vertices.second );
    return symmetric_pair { n1.getNodeId(), n2.getNodeId() };
}
/*******************************************************************/

// запросы
/*******************************************************************/
void QKD_Network::pushRequest()
{
    //std::lock_guard<std::mutex> lock { mLock };
    maRequestQueue.push_back( mRequestGen.genRequest() );
}

Request& QKD_Network::popRequest()
{
    // & - ?
    //std::lock_guard<std::mutex> lock { mLock };
    Request& r = maRequestQueue.front();
    maRequestQueue.pop_front();
    BOOST_LOG_TRIVIAL(trace) << "QKD_Network: Got Request from queue";
    return r;
}

void QKD_Network::processRequest( const Request& req )
{
    //std::lock_guard<std::mutex> lock { mLock };
    Path path = mPathfinder.invokeAlgorithm( req.start, req.dest );
    for ( const auto& l : path.getPathLinkIdList() )
        mKeyGenModel.updateLinkMetrics( l, -1 );
    BOOST_LOG_TRIVIAL(trace) << "QKD_Network: Utilized " << path;
}
/******************************************************************/

std::vector<VertexId> QKD_Network::getTopologyData() const
{
    return mTopology.getTopologyData();
}

std::vector<EdgeId> QKD_Network::getAdjEdgeIds( VertexId v ) const
{
    return mTopology.getAdjEdgeIds( v );
}

void QKD_Network::simulate()
{
    while (true)
    {
        std::this_thread::sleep_for( std::chrono::nanoseconds {std::rand()} );
        pushRequest();
        
        //std::this_thread::sleep_for( std::chrono::nanoseconds {std::rand()} );
        /*
        for ( const auto& [l_id, l_ptr] : mmLinkToId )
            mKeyGenModel.updateLinkMetrics( l_id, 1 );
            
        if ( !maRequestQueue.empty() )
        {
            Request& r = popRequest();
            if ( r.exp_time > dclr::TimeMoment::now() )
            {
                //std::this_thread::sleep_for
                    //( std::chrono::milliseconds {std::rand()} );
                processRequest( r );
            }
            else 
                BOOST_LOG_TRIVIAL(trace) 
                    << "QKD_Network: Disposed of already expired Request ";
        }
        */
    }
}

/*
// private
void QKD_Network::reqGeneration()
{
    while (true)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds {std::rand()} );
        pushRequest();
    }
}

// private
void QKD_Network::keyGeneration()
{
    while (true)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds {std::rand()} );
        for ( const auto& [l_id, l_ptr] : mmLinkToId )
            mKeyGenModel.updateLinkMetrics( l_id, 1 );
    }
}

// private
void QKD_Network::reqProcessing()
{ 
    while (true)
    {
        std::lock_guard<std::mutex> lock { mLock };
        if ( !maRequestQueue.empty() )
        {
            Request& r = popRequest();
            if ( r.exp_time > dclr::TimeMoment::now() )
            {
                std::this_thread::sleep_for
                    ( std::chrono::milliseconds {std::rand()} );
                processRequest( r );
            }
            else 
                BOOST_LOG_TRIVIAL(trace) 
                    << "QKD_Network: Disposed of already expired Request ";
        }
    }
}

std::thread QKD_Network::reqGenerationThr()
{
    return std::thread ( [this]{ reqGeneration(); } );
}

std::thread QKD_Network::keyGenerationThr()
{
    return std::thread ( [this]{ keyGeneration(); } );
}

std::thread QKD_Network::reqProcessingThr()
{
    return std::thread ( [this]{ reqProcessing(); } );
}
*/
