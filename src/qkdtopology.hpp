#ifndef QKDTOPOLOGY_HPP
#define QKDTOPOLOGY_HPP

#include <vector> 
#include <memory> 
#include <iostream>
#include <stdexcept>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp"
#include "qkdrequestgen.hpp"

template <typename NetworkModel>
class QKD_Topology
{
    public:
    
        friend class Vertex;
        friend class Edge;

        template < typename Network >
        friend std::ostream& operator<< (std::ostream& os, 
                                         const QKD_Topology< Network >& t);

    private:

        const NetworkModel& mQKD_Network;

        std::map< VertexId, std::shared_ptr<Vertex>> mmVertexToId;    
        std::map< EdgeId,   std::shared_ptr<Edge>>   mmEdgeToId;

        std::vector< VertexId > maRemovedVertexIdList;
        std::vector< EdgeId >   maRemovedEdgeIdList;

    public:
        
        QKD_Topology () = delete;
        QKD_Topology ( NetworkModel& );

        VertexId addVertex();
        EdgeId   addEdge( VertexId, VertexId );

        void removeVertex( VertexId );
        void removeEdge( EdgeId );

        std::shared_ptr<Vertex> getVertexPtrById( VertexId );
        std::shared_ptr<Edge>   getEdgePtrById( EdgeId );

        Vertex& getVertexById( VertexId );
        Edge&   getEdgeById( EdgeId );
        
        std::vector<EdgeId> getAdjEdgeIds( VertexId );
        std::vector<VertexId> getTopologyData();
};

template < typename NetworkModel >
QKD_Topology<NetworkModel>::QKD_Topology ( NetworkModel& parent )
:
    mQKD_Network { parent }
{}

template < typename NetworkModel >
VertexId QKD_Topology<NetworkModel>::addVertex()
{
    Vertex* pv = new Vertex {};
    VertexId v_id = pv->getVertexId();
    mmVertexToId.emplace( v_id, pv );
    return v_id;
}

template < typename NetworkModel >
EdgeId QKD_Topology<NetworkModel>::addEdge( VertexId v1, VertexId v2 )
{
    Edge* pe = new Edge { getVertexById( v1 ), getVertexById( v2 ) };
    EdgeId e_id = pe->getEdgeId();
    mmEdgeToId.emplace( e_id, pe );
    return e_id; 
}

template < typename NetworkModel >
void QKD_Topology<NetworkModel>::removeVertex( VertexId v )
{
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_id.hasVertex( v ) ) 
        {
            maRemovedEdgeIdList.push_back( edge_id );
            mmEdgeToId.erase( edge_id );
            break;
        }

    auto pv = mmVertexToId.find( v );
    if( pv != mmVertexToId.end() )
    {
        maRemovedVertexIdList.push_back( pv->first );
        mmVertexToId.erase( pv );
    }
}

template < typename NetworkModel >
void QKD_Topology<NetworkModel>::removeEdge( EdgeId e )
{
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_id == e ) 
        {
            -- edge_ptr->first.num_adj_edges; 
            -- edge_ptr->second.num_adj_edges; 
            maRemovedEdgeIdList.push_back( edge_id );
            mmEdgeToId.erase( edge_id );
            break;
        }
}

template < typename NetworkModel >
std::shared_ptr<Vertex> 
QKD_Topology<NetworkModel>::getVertexPtrById( VertexId v )
{
    try {
        std::shared_ptr<Vertex> p = mmVertexToId.at( v );
        return p;
    } catch (...) {
        throw std::out_of_range( "No vertex for given id" );
        //return nullptr;
    }
}

template < typename NetworkModel >
std::shared_ptr<Edge> 
QKD_Topology<NetworkModel>::getEdgePtrById( EdgeId e )
{
    try {
        std::shared_ptr<Edge> p = mmEdgeToId.at( e );
        return p;
    } catch (...) {
        throw std::out_of_range( "No edge for given id" );
        //return nullptr;
    }
}

template < typename NetworkModel >
Vertex& QKD_Topology<NetworkModel>::getVertexById( VertexId v )
{
    return *getVertexPtrById( v );
}

template < typename NetworkModel >
Edge& QKD_Topology<NetworkModel>::getEdgeById( EdgeId e )
{
    return *getEdgePtrById( e );
}

template < typename NetworkModel >
std::vector<EdgeId> QKD_Topology<NetworkModel>::getAdjEdgeIds( VertexId v )
{
    std::vector<EdgeId> adj_edges {};
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_ptr->hasVertex( v ) )
            adj_edges.push_back( edge_id );
    return adj_edges;
}

template <typename NetworkModel>
std::ostream& operator<< ( std::ostream& os, 
                           const QKD_Topology< NetworkModel >& t )
{
    os << "Vertices: ";
    for ( const auto& [key, value] : t.mmVertexToId )
        os << *value << ", ";

    os << '\n';

    os << "Edges: ";
    for ( const auto& [key, value] : t.mmEdgeToId )
        os << *value << ", ";

    return os;
}

template <typename NetworkModel>
std::vector<VertexId> QKD_Topology<NetworkModel>::getTopologyData()
{
    std::vector<VertexId> vert_ids {};
    for ( const auto& [v_id, v_ptr] : mmVertexToId )
        vert_ids.push_back( v_id );
    return vert_ids;
}

#endif  // QKDTOPOLOGY_HPP
