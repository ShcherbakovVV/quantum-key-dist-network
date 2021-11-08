#ifndef QKDPATHFINDER_HPP
#define QKDPATHFINDER_HPP

#include <map>
#include <vector>
#include <utility>
#include <iterator>
#include <functional>

#include <boost/log/trivial.hpp>

#include "qkdlink.hpp"
#include "qkdnode.hpp"

template <typename T1, typename T2, typename Comp = std::less<T2>>
struct ComparePairByValues
{
    bool operator() ( const std::pair<T1, T2>& pr1,
                      const std::pair<T1, T2>& pr2 ) const
    {
        return Comp{} ( pr1.second, pr2.second );
    }
};

template <typename Iter,
          typename Comp = std::less<
    typename std::iterator_traits<Iter>::value_type::second_type
                                    >>
std::pair<typename std::iterator_traits<Iter>::value_type::first_type,
          typename std::iterator_traits<Iter>::value_type::second_type>
MinValuePair( Iter b, Iter e )
{
    using K = typename std::iterator_traits<Iter>::value_type::first_type;
    using V = typename std::iterator_traits<Iter>::value_type::second_type;
    return *std::min_element( b, e, ComparePairByValues<K, V, Comp> {} );
}

template <typename Iter,
          typename Comp = std::less<
    typename std::iterator_traits<Iter>::value_type::second_type
                                    >>
std::pair<typename std::iterator_traits<Iter>::value_type::first_type,
          typename std::iterator_traits<Iter>::value_type::second_type>
MaxValuePair( Iter b, Iter e )
{
    using K = typename std::iterator_traits<Iter>::value_type::first_type;
    using V = typename std::iterator_traits<Iter>::value_type::second_type;
    return *std::max_element( b, e, ComparePairByValues<K, V, Comp> {} );
}

template <typename NetworkModel>
struct DijkstraSP
{
    using Metrics = typename NetworkModel::Metrics;

    static constexpr Metrics METRICS_INF = NetworkModel::METRICS_INF;

    using Edge     = typename NetworkModel::Edge;
    using QKD_Node = typename NetworkModel::QKD_Node;
    using QKD_Link = typename NetworkModel::QKD_Link;

    using VertexId = typename NetworkModel::VertexId;
    using EdgeId   = typename NetworkModel::EdgeId;
    using LinkId   = typename NetworkModel::LinkId;

    using Path = typename NetworkModel::Path;

    Path operator() ( const QKD_Node&, const QKD_Node&, const NetworkModel& );
};

template <typename NetworkModel>
typename DijkstraSP<NetworkModel>::Path
DijkstraSP<NetworkModel>::operator() ( const QKD_Node& st,
                                       const QKD_Node& ds,
                                       const NetworkModel& net )
{
    BOOST_LOG_TRIVIAL(info) << "Invoked DijkstraSP on Nodes: "
        << st << ", " << ds;

    VertexId start_id = st.getVertexId();
    VertexId dest_id  = ds.getVertexId();

    std::map<VertexId, Metrics> unvisited {};
    for ( const auto& x : net.getTopologyData() )
        unvisited[x] = METRICS_INF;
    unvisited[start_id] = 0;

    VertexId current_id = start_id;
    std::vector<VertexId> path_vertices { current_id };
    std::vector<VertexId> blacklist_vertices {};

    while ( !unvisited.empty() )
    {
        int candidate_count = 0;
        // итерации по смежным вершине ребрам
        for ( const auto& e : net.getAdjEdgeIds( current_id ) )
        {
            QKD_Link& this_link = net.getLinkByEdgeId( e );
            Metrics this_metrics = this_link.getMetricsValue();
            if ( this_metrics == 0 ) continue;  // если на линке нет ключей

            Metrics distance = 1.0 / this_metrics;
            Metrics current_distance = unvisited.at( current_id );
            Metrics assign_distance = current_distance + distance;

            Edge& this_edge = net.getEdgeById( e );
            VertexId neighbor_id = this_edge.getOtherVertexId( current_id );
            try {
                if ( unvisited.at( neighbor_id ) > assign_distance )
                    unvisited.at( neighbor_id ) = assign_distance;
            } catch ( std::out_of_range& exc ) { continue; }
            ++ candidate_count;
        }
        if ( candidate_count == 0  // не смогли продвинуться дальше
                                   // из-за нехватки ключей
             && std::min_element( unvisited.begin(),
                                  unvisited.end() )->second == METRICS_INF )
        {
            BOOST_LOG_TRIVIAL(info) << "Aborting DijkstraSP";
            throw std::runtime_error
                { "Can't navigate a Path: no connection or Quantum Keys" };
        }
        unvisited.erase( current_id );
        current_id = MinValuePair( unvisited.begin(), unvisited.end() ).first;
        path_vertices.push_back( current_id );

        if ( current_id == dest_id ) break;
    }

    // получим линки между вершинами для составления пути
    Path path { st.getNodeId(), ds.getNodeId() };
    int links_between = 0;  // количество несвязных вершин между двумя связными
                            // в path_vertices
    for ( auto i = path_vertices.rbegin(); i != path_vertices.rend()-1; ++i )
    {
        // выбрасываем из path_vertices ненужные вершины
        std::vector<EdgeId> edge_ids {};
        try {
            edge_ids = net.getAdjEdgeIds( *(i - links_between), *(i + 1) );
        } catch ( std::out_of_range& exc ) {
            ++ links_between;
            continue;
        }
        links_between = 0;

        std::vector<QKD_Link> links;
        for ( const auto& e : edge_ids )
            links.push_back( net.getLinkByEdgeId( e ) );

        LinkId path_link = std::min_element( links.begin(),
                                             links.end() )->getLinkId();
        path.addLinkIdToPathFront( path_link );
    }

    return path;
}

template <typename NetworkModel>
class QKD_Pathfinder
{
public:

    template <typename NetPtr, typename ModPtr>
        friend void assignParent( NetPtr, ModPtr );

    using NodeId = typename NetworkModel::NodeId;
    using LinkId = typename NetworkModel::LinkId;

    using Path = typename NetworkModel::Path;

private:

    NetworkModel* mpQKD_Network = nullptr;

public:

    QKD_Pathfinder ();

    ~QKD_Pathfinder ();

    template <typename Algorithm>
    Path invokeAlgorithm( NodeId st, NodeId ds ) const;
};

template <typename NetworkModel>
QKD_Pathfinder<NetworkModel>::QKD_Pathfinder ()
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_Pathfinder";
}

template <typename NetworkModel>
QKD_Pathfinder<NetworkModel>::~QKD_Pathfinder ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_Pathfinder";
}

template <typename NetworkModel>
template <typename Algorithm>
typename QKD_Pathfinder<NetworkModel>::Path
QKD_Pathfinder<NetworkModel>::invokeAlgorithm ( NodeId st, NodeId ds ) const
{
    Algorithm alg {};
    return alg( mpQKD_Network->getNodeById( st ),
                mpQKD_Network->getNodeById( ds ),
                *mpQKD_Network );
}

#endif  // QKDPATHFINDER_HPP
