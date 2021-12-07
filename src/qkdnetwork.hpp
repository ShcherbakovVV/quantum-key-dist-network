#ifndef QKDNETWORK_HPP
#define QKDNETWORK_HPP

#include <map>
#include <string>
#include <thread>
#include <chrono>
#include <utility>
#include <cstring>
#include <fstream>
#include <optional>
#include <stdexcept>

#if defined(__linux__)
    #include <unistd.h>
    #include <sys/wait.h>
    //#include <sys/types.h>
#elif defined(_WIN64)
    #include <windows.h>
#endif

#include <boost/log/trivial.hpp>
#include <boost/circular_buffer.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

extern const char* gv_file;
extern const char* img_file;
extern const char* bg_file;
extern SDL_Window* window;

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

    #ifdef THREAD_BASED
        std::mutex mMutex;
    #endif

    std::shared_ptr<QKD_Topology<this_type>>    mpTopology;
    std::shared_ptr<QKD_Pathfinder<this_type>>  mpPathfinder;
    std::shared_ptr<QKD_RequestGen<this_type>>  mpRequestGen;
    std::shared_ptr<QKD_KeyGenModel<this_type>> mpKeyGenModel;

    std::map<NodeId, std::shared_ptr<QKD_Node>> mmNodeToId;
    std::map<LinkId, std::shared_ptr<QKD_Link>> mmLinkToId;

    std::vector<NodeId> maRemovedNodeIdList;
    std::vector<LinkId> maRemovedLinkIdList;

    std::vector<NodeId> maAuxNodeIdList;

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
    NodeId addNode( NodeClass, std::string lbl = makeNodeLabel() );  // строка&?
    LinkId addLink( NodeId, NodeId, Metrics m );

    void removeNode( NodeId );
    void removeLink( LinkId );

    // получение информации
    bool isNodeRemoved( NodeId ) const;
    bool isLinkRemoved( LinkId ) const;

    bool isAuxNode( NodeId ) const;

    bool isNodeInPath( const Path&, NodeId ) const;
    bool isLinkInPath( const Path& path, LinkId l ) const
        { return path.isLinkInPath( l ); };

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
    template <typename Alg> bool processRequest( Request& );

    #ifdef THREAD_BASED
        std::thread keyGenThread()
            { return std::thread ( [this]{ keyGeneration(); } ); }
        std::thread reqGenThread()
            { return std::thread ( [this]{ reqGeneration(); } ); }
        template <typename Alg> std::thread reqProcThread()
            { return std::thread ([this]{ reqProcessing<Alg>();} ); }
    #endif  // THREAD_BASED

    // GraphViz
    std::string toGraphViz( std::optional<Path> ) const;
    void displayNetwork( std::string ) const;
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
    return label + std::to_string( QKD_Node::getLastNodeId().value() + 1 );
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
typename QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::NodeId
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::addNode( NodeClass ncls, std::string lbl )
{
    VertexId v = mpTopology->addVertex();
    QKD_Node* pn = new QKD_Node { mpTopology->getVertexPtrById( v ), lbl };
    NodeId n_id = pn->getNodeId();
    mmNodeToId.insert( std::make_pair( n_id, pn ) );
    if ( ncls == NodeClass::Aux )
        maAuxNodeIdList.push_back( n_id );
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
            if ( isAuxNode( node_id ) )
                maAuxNodeIdList.erase( node_id );
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
    const auto& b = maRemovedNodeIdList.begin();
    const auto& e = maRemovedNodeIdList.end();
    if ( std::find( b, e, n ) != e )
        return true;
    return false;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
bool QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::isLinkRemoved( LinkId l ) const
{
    const auto& b = maRemovedLinkIdList.begin();
    const auto& e = maRemovedLinkIdList.end();
    if ( std::find( b, e, l ) != e )
        return true;
    return false;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
bool QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::isAuxNode( NodeId n ) const
{
    const auto& b = maAuxNodeIdList.begin();
    const auto& e = maAuxNodeIdList.end();
    if ( std::find( b, e, n ) != e )
        return true;
    return false;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
bool QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::isNodeInPath
( const Path& path, NodeId n ) const
{
    if ( path.start == n || path.dest == n )
        return true;
    auto& links = path.getPathLinkIdList();
    for ( const auto& l : links )
    {
        Edge& e = getLinkById(l).getEdge();
        VertexId vi = getNodeById(n).getVertexId();
        if ( e.hasVertex( vi ) )
            return true;
    }
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
            { "No QKD_Node for given NodeId: " + std::to_string( n.value() ) };
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
            { "No QKD_Link for given LinkId: " + std::to_string( l.value() ) };
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
    #ifdef THREAD_BASED
        std::lock_guard lock { mMutex };
    #endif  // THREAD_BASED

    for ( const auto& [l_id, l_ptr] : mmLinkToId )
        if ( static_cast<double>(std::rand())/RAND_MAX > 0.5 )
            mpKeyGenModel->updateLinkMetrics( l_id, 1 );
    displayNetwork( toGraphViz( std::nullopt ) );
    BOOST_LOG_TRIVIAL(info) << "QKD_Network: Generated Quantum Keys";
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::pushRequest()
{
    #ifdef THREAD_BASED
        std::lock_guard lock { mMutex };
    #endif  // THREAD_BASED

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
    #ifdef THREAD_BASED
        std::lock_guard lock { mMutex };
    #endif  // THREAD_BASED

    if ( !maRequestQueue.empty() )
    {
        Request& r = maRequestQueue.pop_front();  // & - ?
        BOOST_LOG_TRIVIAL(info) << "QKD_Network: Got Request from Queue";
        return r;
    }
    throw std::runtime_error {"Request Queue is empty"};
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
template <typename Alg>
bool QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::processRequest( Request& req )
{
    #ifdef THREAD_BASED
        std::lock_guard lock { mMutex };
    #endif  // THREAD_BASED

    if ( req.exp_time < TimePoint {} )  // если заявка устарела
    {
        BOOST_LOG_TRIVIAL(info) << "QKD_Network: Discarding outdated " << req;
        return false;  
    }

    try {
        Path path =
            mpPathfinder->template invokeAlgorithm<Alg>( req.start, req.dest );
        for ( const auto& l : path.getPathLinkIdList() )
            mpKeyGenModel->updateLinkMetrics( l, -1 );
        if ( !path.getPathLinkIdList().empty() )
            BOOST_LOG_TRIVIAL(info) << "QKD_Network: Utilized " << path;
        displayNetwork( toGraphViz( path ) );
    } catch ( std::exception& e ) {
        BOOST_LOG_TRIVIAL(error) << e.what();
    }
    return true;
}

#ifdef THREAD_BASED

    template <ClockType Clk, DurationType Dur,
              std::uniform_random_bit_generator Eng, typename Dist,
              typename Mtr, std::unsigned_integral Int>
    void QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::keyGeneration()
    {
        while (true)
        {
            std::this_thread::sleep_for( std::chrono::nanoseconds
                { std::rand()} );
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
                { std::rand()} );
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
            std::this_thread::sleep_for
                ( std::chrono::nanoseconds { std::rand() } );
            processRequest<Alg>( r );  //  TODO: now bool instead of void
        } catch ( std::runtime_error& re ) {
            BOOST_LOG_TRIVIAL(info) << re.what();
            std::this_thread::sleep_for
                ( std::chrono::nanoseconds { std::rand() } );
        }
    }

#endif  // THREAD_BASED

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
std::string
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::toGraphViz
( std::optional<Path> path_opt )
const
{
    std::string gv = "graph G {\nbgcolor=\"transparent\"\n";
    for ( const auto& npr : mmNodeToId )  // вывод объявлений узлов
    {
        gv += "\"" + npr.second->label + "\"[pos=\""
            + std::to_string( npr.second->xpos ) + ","
            + std::to_string( npr.second->ypos ) + "!\""
            + "fontname = Arial";
        if ( !isAuxNode( npr.first ) )
            gv += ",shape=box3d";
        else
            gv += ",shape=rect";
        if ( path_opt && isNodeInPath( path_opt.value(), npr.first ) )
        {
            gv += ",penwidth=5";
            if ( npr.second->getNodeId() == path_opt.value().start
                 || npr.second->getNodeId() == path_opt.value().dest )
                gv += ",style=filled,fillcolor=gray50";
        }
        gv += "];\n";
    }
    for ( const auto& lpr : mmLinkToId )  // вывод линков
    {
        QKD_Link l = *lpr.second;
        symmetric_pair<VertexId> vpr = l.getEdge().getAdjVertexIds();
        QKD_Node& n1 = getNodeByVertexId( vpr.first );
        QKD_Node& n2 = getNodeByVertexId( vpr.second );
        std::string mtr_lbl = std::to_string( static_cast<int>( l.getMetricsValue() ) );
        gv += "\"" + n1.label + "\"--\"" + n2.label + "\"[label=\"" + mtr_lbl + "  \"";
        if ( path_opt && path_opt.value().isLinkInPath( lpr.first ) )
            gv += ",penwidth=5";
        gv += ",labeljust=l,fontname = Arial,fontsize=16];\n";
    }
    gv += '}';
    return gv;
}

template <ClockType Clk, DurationType Dur,
          std::uniform_random_bit_generator Eng, typename Dist,
          typename Mtr, std::unsigned_integral Int>
void
QKD_Network<Clk, Dur, Eng, Dist, Mtr, Int>::displayNetwork( std::string net )
const
{
    std::fstream fstrm { gv_file, std::ios_base::out };
    fstrm << net;
    fstrm.close();

    std::string img_file_str { "-o" };
    const char* img_file2 = (img_file_str += img_file).c_str();

    #if defined(__linux__)
        pid_t pid1 = vfork();
        if ( pid1 == 0 )  // дочерний процесс
        {
            pid_t pid2 = vfork();
            if ( pid2 == 0 )
                execl( "/bin/neato", "neato", gv_file,
                       R"(-Gsize=10,7/!)", "-Gdpi=100", "-Gratio=fill",
                       "-Tpng", img_file2, (char*) nullptr );
            else if ( pid2 > 0 )
            {
                wait( nullptr );  // ждем завершения дочернего процесса
                execl( "/bin/convert", "convert", bg_file, img_file,
                       "-background", "white", "-compose", "over", "-layers",
                       "flatten", img_file, (char*) nullptr );
            }
        }
        else if ( pid1 > 0 )  // родитель
        {
            wait( nullptr );
            SDL_Surface* wndbg = SDL_GetWindowSurface( window );
            SDL_Surface* image = IMG_Load( img_file );
            if ( image == nullptr )
            {
                std::cerr << "SDL_image error: " << IMG_GetError() << '\n';
                return;
            }
            int blit = SDL_BlitSurface( image, nullptr, wndbg, nullptr );
            if ( blit < 0 )
            {
                std::cerr << "SDL error: " << SDL_GetError() << '\n';
                return;
            }
            SDL_UpdateWindowSurface( window );
            SDL_FreeSurface( wndbg );
            SDL_FreeSurface( image );
        }
    #elif defined(_WIN64)
        // windows specific syscalls
    #endif
}

#endif  // QKDNETWORK_HPP
