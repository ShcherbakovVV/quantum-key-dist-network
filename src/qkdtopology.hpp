#ifndef QKDTOPOLOGY_HPP
#define QKDTOPOLOGY_HPP

#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <stdexcept>

#include <boost/log/trivial.hpp>

#include "edge.hpp"
#include "vertex.hpp"
#include "qkdrequestgen.hpp"

template <typename NetworkModel>
class QKD_Topology
{
public:

    template <typename NetPtr, typename ModPtr>
        friend void assignParent( NetPtr, ModPtr );

    using Vertex = typename NetworkModel::Vertex;
    using Edge   = typename NetworkModel::Edge;

    using VertexId = typename NetworkModel::VertexId;
    using EdgeId   = typename NetworkModel::EdgeId;

private:

    NetworkModel* mpQKD_Network = nullptr;

    // константные указатели?
    std::map<VertexId, std::shared_ptr<Vertex>> mmVertexToId;
    std::map<EdgeId,   std::shared_ptr<Edge>>   mmEdgeToId;

    // константные Id?
    std::vector<VertexId> maRemovedVertexIdList;
    std::vector<EdgeId>   maRemovedEdgeIdList;

public:

    QKD_Topology ();

    ~QKD_Topology ();

    VertexId addVertex();
    EdgeId   addEdge( VertexId, VertexId );

    void removeVertex( VertexId );
    void removeEdge( EdgeId );

    const std::shared_ptr<Vertex>& getVertexPtrById( VertexId ) const;
    const std::shared_ptr<Edge>&   getEdgePtrById( EdgeId ) const;

    Vertex& getVertexById( VertexId ) const;
    Edge&   getEdgeById( EdgeId ) const;

    std::vector<EdgeId>   getAdjEdgeIds( VertexId ) const;
    std::vector<EdgeId>   getAdjEdgeIds( VertexId, VertexId ) const;
    std::vector<VertexId> getTopologyData() const;
};

template <typename NetworkModel>
QKD_Topology<NetworkModel>::QKD_Topology ()
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_Topology";
}

template <typename NetworkModel>
QKD_Topology<NetworkModel>::~QKD_Topology ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_Topology";
}

template <typename NetworkModel>
typename QKD_Topology<NetworkModel>::VertexId
QKD_Topology<NetworkModel>::addVertex()
{
    Vertex* pv = new Vertex {};
    VertexId v_id = pv->getVertexId();
    mmVertexToId.insert( std::make_pair( v_id, pv ) );
    BOOST_LOG_TRIVIAL(trace) << "QKD_Topology: Added " << *pv;
    return v_id;
}

template <typename NetworkModel>
typename QKD_Topology<NetworkModel>::EdgeId
QKD_Topology<NetworkModel>::addEdge( VertexId v1, VertexId v2 )
{
    Edge* pe = new Edge { getVertexById(v1), getVertexById(v2) };
    EdgeId e_id = pe->getEdgeId();
    mmEdgeToId.insert( std::make_pair( e_id, pe ) );
    BOOST_LOG_TRIVIAL(trace) << "QKD_Topology: Added " << *pe;
    return e_id;
}

template <typename NetworkModel>
void QKD_Topology<NetworkModel>::removeVertex( VertexId v )
{
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_id.hasVertex( v ) )
        {
            BOOST_LOG_TRIVIAL(trace) << "QKD_Topology: Removed " << *edge_ptr;
            maRemovedEdgeIdList.push_back( edge_id );
            mmEdgeToId.erase( edge_id );
            break;
        }

    auto pv = mmVertexToId.find( v );
    if( pv != mmVertexToId.end() )
    {
        BOOST_LOG_TRIVIAL(trace) << "QKD_Topology: Removed orphan Edge";
        maRemovedVertexIdList.push_back( pv->first );
        mmVertexToId.erase( pv );
    }
}

template <typename NetworkModel>
void QKD_Topology<NetworkModel>::removeEdge( EdgeId e )
{
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_id == e )
        {
            BOOST_LOG_TRIVIAL(trace) << "QKD_Topology: Removed " << *edge_ptr;
            -- edge_ptr->mpVertex1->num_adj_edges;
            -- edge_ptr->mpVertex2->num_adj_edges;
            maRemovedEdgeIdList.push_back( edge_id );
            mmEdgeToId.erase( edge_id );
            break;
        }
}

template <typename NetworkModel>
const std::shared_ptr<typename QKD_Topology<NetworkModel>::Vertex>&
QKD_Topology<NetworkModel>::getVertexPtrById( VertexId v ) const
{
    try {
        return mmVertexToId.at( v );
    } catch (...) {
        throw std::out_of_range
            { "No Vertex for given VertexId: " + std::to_string( v.value() ) };
    }
}

template <typename NetworkModel>
const std::shared_ptr<typename QKD_Topology<NetworkModel>::Edge>&
QKD_Topology<NetworkModel>::getEdgePtrById( EdgeId e ) const
{
    try {
        return mmEdgeToId.at( e );
    } catch (...) {
        throw std::out_of_range
            { "No Edge for given EdgeId: " + std::to_string( e.value() ) };
    }
}

template <typename NetworkModel>
typename QKD_Topology<NetworkModel>::Vertex&
QKD_Topology<NetworkModel>::getVertexById( VertexId v ) const
{
    return *getVertexPtrById( v );
}

template <typename NetworkModel>
typename QKD_Topology<NetworkModel>::Edge&
QKD_Topology<NetworkModel>::getEdgeById( EdgeId e ) const
{
    return *getEdgePtrById( e );
}

template <typename NetworkModel>
std::vector<typename QKD_Topology<NetworkModel>::EdgeId>
QKD_Topology<NetworkModel>::getAdjEdgeIds( VertexId v ) const
{
    std::vector<EdgeId> adj_edges {};
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_ptr->hasVertex( v ) )
            adj_edges.push_back( edge_id );
    if ( !adj_edges.empty() )
        return adj_edges;
    throw std::out_of_range {"No adjacent Edges for given Vertex"};
}

template <typename NetworkModel>
std::vector<typename QKD_Topology<NetworkModel>::EdgeId>
QKD_Topology<NetworkModel>::getAdjEdgeIds( VertexId v1, VertexId v2 ) const
{
    std::vector<EdgeId> adj_edges {};
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_ptr->hasVertices( v1, v2 ) )
            adj_edges.push_back( edge_id );
    if ( !adj_edges.empty() )
            return adj_edges;
    throw std::out_of_range {"No adjacent Edges for given Vertices"};
}

template <typename NetworkModel>
std::vector<typename QKD_Topology<NetworkModel>::VertexId>
QKD_Topology<NetworkModel>::getTopologyData() const
{
    std::vector<VertexId> vert_ids {};
    for ( const auto& [v_id, v_ptr] : mmVertexToId )
        vert_ids.push_back( v_id );
    if ( !vert_ids.empty() )
        return vert_ids;
    throw std::runtime_error {"Topology info: no Vertices in graph"};
}

#endif  // QKDTOPOLOGY_HPP
