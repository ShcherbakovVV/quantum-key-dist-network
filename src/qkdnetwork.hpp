#ifndef QKDNETWORK_HPP
#define QKDNETWORK_HPP

#include <map>
#include <string>
#include <thread>
#include <chrono>
#include <utility>
#include <stdexcept>

#include <boost/log/trivial.hpp>
#include <boost/circular_buffer.hpp>

#include "lib/timemoment.hpp"
#include "lib/limited_queue.hpp"
#include "lib/symmetric_pair.hpp"

#include "path.hpp"
#include "edge.hpp"
#include "vertex.hpp"
#include "qkdlink.hpp"
#include "qkdnode.hpp"
#include "request.hpp"
#include "qkdtopology.hpp"
#include "qkdpathfinder.hpp"
#include "qkdrequestgen.hpp"
#include "qkdkeygenmodel.hpp"

using std::chrono_literals::operator""ms;

template <typename NetworkPtr, typename ModulePtr>
void assignParent( NetworkPtr parent, ModulePtr module )
{
    module->mpQKD_Network = parent;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
class QKD_Network
{
public:

    using this_type = QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>;

    using Clock      = Clk;
    using Duration   = Dur;
    using RNG_Engine = Eng;
    using IntDistrib = Dist;
    using Metrics    = Mtr;
    using IdInt      = Int;

    static constexpr Metrics METRICS_INF = std::numeric_limits<Metrics>::max();

    using Vertex   = VertexT<IdInt>;
    using Edge     = EdgeT<IdInt>;
    using QKD_Node = QKD_NodeT<IdInt>;
    using QKD_Link = QKD_LinkT<IdInt, Metrics>;

    using VertexId = typename Vertex::id_type;
    using EdgeId   = typename Edge::id_type;
    using NodeId   = typename QKD_Node::id_type;
    using LinkId   = typename QKD_Link::id_type;

    using TimePoint = TimeMoment<Clock, Duration>;
    using IntRNG    = UIntRandNumGen<IdInt, RNG_Engine, IntDistrib>;

    using Path    = PathT<IdInt, Metrics>;
    using Request = RequestT<this_type>;

private:

    std::mutex mMutex;

    std::shared_ptr<QKD_Topology<this_type>>    mpTopology;
    std::shared_ptr<QKD_Pathfinder<this_type>>  mpPathfinder;
    std::shared_ptr<QKD_RequestGen<this_type>>  mpRequestGen;
    std::shared_ptr<QKD_KeyGenModel<this_type>> mpKeyGenModel;

    std::map<NodeId, std::shared_ptr<QKD_Node>> mmNodeToId;
    std::map<LinkId, std::shared_ptr<QKD_Link>> mmLinkToId;

    std::vector<NodeId> maRemovedNodeIdList;
    std::vector<LinkId> maRemovedLinkIdList;

    limited_queue<Request> maRequestQueue;

    static std::string makeNodeLabel();

    void reqGeneration();
    void keyGeneration();
    template <typename Alg> void reqProcessing();

public:

    QKD_Network ( QKD_Topology<this_type>*,
                  QKD_Pathfinder<this_type>*,
                  QKD_RequestGen<this_type>*,
                  QKD_KeyGenModel<this_type>*,
                  std::size_t qcap );

    ~QKD_Network ()
        { BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_Network"; }

    // модификация сети
    NodeId addNode( std::string lbl = makeNodeLabel() );  // строка&?
    LinkId addLink( NodeId, NodeId, Metrics m );

    void removeNode( NodeId );
    void removeLink( LinkId );

    // получение информации
    bool isNodeRemoved( NodeId ) const;
    bool isLinkRemoved( LinkId ) const;

    Edge& getEdgeById( EdgeId e ) const
        { return mpTopology->getEdgeById( e ); }
    Vertex& getVertexById( VertexId v ) const
        { return mpTopology->getVertexById( v ); }

    const std::shared_ptr<QKD_Node>& getNodePtrById( NodeId ) const;
    const std::shared_ptr<QKD_Link>& getLinkPtrById( LinkId ) const;

    QKD_Node& getNodeById( NodeId n ) const
        { return *getNodePtrById( n ); }
    QKD_Link& getLinkById( LinkId l ) const
        { return *getLinkPtrById( l ); }

    QKD_Node& getNodeByVertex( const Vertex& ) const;
    QKD_Link& getLinkByEdge( const Edge& ) const;

    QKD_Node& getNodeByVertexId( VertexId v ) const
        { return getNodeByVertex( mpTopology->getVertexById( v ) ); }
    QKD_Link& getLinkByEdgeId( EdgeId e ) const
        { return getLinkByEdge( mpTopology->getEdgeById( e ) ); }

    NodeId getNodeIdByVertexId( VertexId v ) const
        { return getNodeByVertexId( v ).getNodeId(); }
    LinkId getLinkIdByEdgeId( EdgeId e ) const
        { return getLinkByEdgeId( e ).getLinkId(); }

    // Metrics getMetricsByLinkId( LinkId l ) const
    //     { return getLinkById().getMetricsValue(); }

    symmetric_pair<NodeId> getAdjNodeIds( LinkId ) const;

    // получение информации для алгоритмов
    std::vector<VertexId> getTopologyData() const
        { return mpTopology->getTopologyData(); }

    std::vector<EdgeId> getAdjEdgeIds( VertexId v ) const
        { return mpTopology->getAdjEdgeIds( v ); }
    std::vector<EdgeId> getAdjEdgeIds( VertexId v1, VertexId v2 ) const
        { return mpTopology->getAdjEdgeIds( v1, v2 ); }

    // симуляция
    void     genQuantumKeys();
    void     pushRequest();
    Request& popRequest();
    template <typename Alg> void processRequest( Request& );

    std::thread keyGenThread()
        { return std::thread ( [this]{ keyGeneration(); } ); }
    std::thread reqGenThread()
        { return std::thread ( [this]{ reqGeneration(); } ); }
    template <typename Alg> std::thread reqProcThread()
        { return std::thread ([this]{ reqProcessing<Alg>();} ); }
};

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::QKD_Network (
    QKD_Topology<this_type>* top,
    QKD_Pathfinder<this_type>* pthf,
    QKD_RequestGen<this_type>* rqsg,
    QKD_KeyGenModel<this_type>* kgmd,
    std::size_t queue_cap )
:
    mpTopology { top },
    mpPathfinder { pthf },
    mpRequestGen { rqsg },
    mpKeyGenModel { kgmd },
    maRequestQueue { queue_cap }
{
    assignParent( this, mpTopology );
    assignParent( this, mpPathfinder );
    assignParent( this, mpRequestGen );
    assignParent( this, mpKeyGenModel );
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_Network";
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
std::string QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::makeNodeLabel()
{
    std::string label = "Node";
    return label + std::to_string( QKD_Node::getLastNodeId().value()+1 );
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::NodeId
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::addNode( std::string lbl )
{
    VertexId v = mpTopology->addVertex();
    QKD_Node* pn = new QKD_Node { mpTopology->getVertexPtrById( v ), lbl };
    NodeId n_id = pn->getNodeId();
    mmNodeToId.insert( std::make_pair( n_id, pn ) );
    mpRequestGen->updGenParams();
    BOOST_LOG_TRIVIAL(info) << "QKD_Network: Added " << *pn;
    return n_id;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::LinkId
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::addLink( NodeId n1, NodeId n2,
                                                     Metrics m )
{
    EdgeId e = mpTopology->addEdge( getNodeById(n1).getVertexId(),
                                    getNodeById(n2).getVertexId() );
    QKD_Link* pl = new QKD_Link { mpTopology->getEdgePtrById( e ), m };
    LinkId l_id = pl->getLinkId();
    mmLinkToId.insert( std::make_pair( l_id, pl ) );
    mpRequestGen->updGenParams();
    BOOST_LOG_TRIVIAL(info) << "QKD_Network: Added " << *pl;
    return l_id;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::removeNode( NodeId n )
{
    for ( const auto& [node_id, node_ptr] : mmNodeToId )
        if ( node_id == n )
        {
            // должно стоять в конце блока
            BOOST_LOG_TRIVIAL(info) << "QKD_Network: Removed " << *node_ptr;
            maRemovedNodeIdList.push_back( node_id );
            mmNodeToId.erase( node_id );
            break;
        }
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::removeLink( LinkId l )
{
    for ( const auto& [link_id, link_ptr] : mmLinkToId )
        if ( link_id == l )
        {
            BOOST_LOG_TRIVIAL(info) << "QKD_Network: Removed " << *link_ptr;
            maRemovedLinkIdList.push_back( link_id );
            mmLinkToId.erase( link_id );
            break;
        }
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
bool QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::isNodeRemoved( NodeId n ) const
{
    const auto b = maRemovedNodeIdList.begin();
    const auto e = maRemovedNodeIdList.end();
    if ( std::find( b, e, n ) != e )
        return true;
    return false;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
bool QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::isLinkRemoved( LinkId l ) const
{
    const auto b = maRemovedLinkIdList.begin();
    const auto e = maRemovedLinkIdList.end();
    if ( std::find( b, e, l ) != e )
        return true;
    return false;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
const std::shared_ptr
    <typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::QKD_Node>&
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::getNodePtrById( NodeId n ) const
{
    try {
        return mmNodeToId.at( n );
    } catch (...) {
        throw std::out_of_range
            { "No QKD_Node for given NodeId: " + to_string(n) };
    }
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
const std::shared_ptr
    <typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::QKD_Link>&
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::getLinkPtrById( LinkId l ) const
{
    try {
        return mmLinkToId.at( l );
    } catch (...) {
        throw std::out_of_range
            { "No QKD_Link for given LinkId: " + to_string(l) };
    }
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::QKD_Node&
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::getNodeByVertex( const Vertex& v )
const
{
    for ( const auto& [node_id, node_ptr] : mmNodeToId )
        if ( *(node_ptr->mpVertex) == v )
            return *node_ptr;
    throw std::out_of_range { "No QKD_Node for given Vertex&" };
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::QKD_Link&
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::getLinkByEdge( const Edge& e )
const
{
    for ( const auto& [link_id, link_ptr] : mmLinkToId )
        if ( *(link_ptr->mpEdge) == e )
            return *link_ptr;
    throw std::out_of_range { "No QKD_Link for given Edge&" };
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
symmetric_pair<typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::NodeId>
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::getAdjNodeIds( LinkId l ) const
{
    QKD_Link& link = getLinkById( l );
    Edge& edge = link.getEdge();
    symmetric_pair<VertexId> vertices = edge.getAdjVertexIds();
    QKD_Node& n1 = getNodeByVertexId( vertices.first );
    QKD_Node& n2 = getNodeByVertexId( vertices.second );
    return symmetric_pair { n1.getNodeId(), n2.getNodeId() };
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::genQuantumKeys()
{
    std::lock_guard lock { mMutex };
    for ( const auto& [l_id, l_ptr] : mmLinkToId )
        mpKeyGenModel->updateLinkMetrics( l_id, 1 );
    BOOST_LOG_TRIVIAL(info) << "QKD_Network: Generated Quantum Keys";
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::pushRequest()
{
    std::lock_guard lock { mMutex };
    if ( maRequestQueue.try_push( mpRequestGen->genRequest() ) == 0 )
        BOOST_LOG_TRIVIAL(info) << "QKD_Network: Request Queue is full";
    else
        BOOST_LOG_TRIVIAL(info) << "QKD_Network: Added Request to Queue";
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::Request&
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::popRequest()
{
    std::lock_guard lock { mMutex };
    if ( !maRequestQueue.empty() )
    {
        Request& r = maRequestQueue.pop_front();  // & - ?
        BOOST_LOG_TRIVIAL(info) << "QKD_Network: Got Request from queue";
        return r;
    }
    throw std::runtime_error {"Request queue is empty"};
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
template <typename Alg>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::processRequest( Request& req )
{
    std::lock_guard lock { mMutex };
    try {
        Path path =
            mpPathfinder->template invokeAlgorithm<Alg>( req.start, req.dest );
        for ( const auto& l : path.getPathLinkIdList() )
            mpKeyGenModel->updateLinkMetrics( l, -1 );
        BOOST_LOG_TRIVIAL(info) << "QKD_Network: Utilized " << path;
    } catch ( std::exception& e ) {
        BOOST_LOG_TRIVIAL(error) << e.what();
    }
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::keyGeneration()
{
    while (true)
    {
        std::this_thread::sleep_for( std::chrono::nanoseconds
            { 10*std::rand()} );
        genQuantumKeys();
    }
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::reqGeneration()
{
    while (true)
    {
        std::this_thread::sleep_for( std::chrono::nanoseconds
            { 100*std::rand()} );
        pushRequest();
    }
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
template <typename Alg>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::reqProcessing()
{
    while (true)
    try {
        Request& r = popRequest();
        if ( r.exp_time > TimePoint {} )
        {
            std::this_thread::sleep_for
                ( std::chrono::nanoseconds { 10*std::rand() } );
            processRequest<Alg>( r );
        }
    } catch ( std::runtime_error& re ) {
        BOOST_LOG_TRIVIAL(info) << re.what();
        std::this_thread::sleep_for
            ( std::chrono::nanoseconds { 1000*std::rand() } );
    }
}

#endif  // QKDNETWORK_HPP
