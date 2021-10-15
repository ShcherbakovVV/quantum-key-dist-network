#ifndef QKDTOPOLOGY_HPP
#define QKDTOPOLOGY_HPP

#include <vector> 
#include <memory> 
#include <iostream>
#include <stdexcept>

#include <boost/log/trivial.hpp>

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

        template <typename Network>
        friend std::ostream& operator<< (std::ostream& os, 
                                         const QKD_Topology<Network>& t);

    private:

        NetworkModel& mQKD_Network;

        // константные указатели?
        std::map<VertexId, std::shared_ptr<Vertex>> mmVertexToId;    
        std::map<EdgeId,   std::shared_ptr<Edge>>   mmEdgeToId;

        // константные Id?
        std::vector<VertexId> maRemovedVertexIdList;
        std::vector<EdgeId>   maRemovedEdgeIdList;

    public:
        
        QKD_Topology () = delete;
        QKD_Topology ( NetworkModel& );
        
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
        std::vector<VertexId> getTopologyData() const;
};

template < typename NetworkModel >
QKD_Topology<NetworkModel>::QKD_Topology ( NetworkModel& parent )
:
    mQKD_Network { parent }
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_Topology";
}

template < typename NetworkModel >
QKD_Topology<NetworkModel>::~QKD_Topology ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_Topology";
}

template < typename NetworkModel >
VertexId QKD_Topology<NetworkModel>::addVertex()
{
    Vertex* pv = new Vertex {};
    VertexId v_id = pv->getVertexId();
    mmVertexToId.emplace( v_id, pv );
    BOOST_LOG_TRIVIAL(trace) << "QKD_Topology: Added " << *pv;
    return v_id;
}

template < typename NetworkModel >
EdgeId QKD_Topology<NetworkModel>::addEdge( VertexId v1, VertexId v2 )
{
    Edge* pe = new Edge { getVertexById( v1 ), getVertexById( v2 ) };
    EdgeId e_id = pe->getEdgeId();
    mmEdgeToId.emplace( e_id, pe );
    BOOST_LOG_TRIVIAL(trace) << "QKD_Topology: Added " << *pe;
    return e_id; 
}

template < typename NetworkModel >
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

template < typename NetworkModel >
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

template < typename NetworkModel >
const std::shared_ptr<Vertex>&
QKD_Topology<NetworkModel>::getVertexPtrById( VertexId v ) const
{
    try {
        return mmVertexToId.at( v );
    } catch (...) {
        throw std::out_of_range( "No Vertex for given Id" );
    }
}

template < typename NetworkModel >
const std::shared_ptr<Edge>&
QKD_Topology<NetworkModel>::getEdgePtrById( EdgeId e ) const
{
    try {
        return mmEdgeToId.at( e );
    } catch (...) {
        throw std::out_of_range( "No Edge for given Id" );
    }
}

template < typename NetworkModel >
Vertex& QKD_Topology<NetworkModel>::getVertexById( VertexId v ) const
{
    return *getVertexPtrById( v );
}

template < typename NetworkModel >
Edge& QKD_Topology<NetworkModel>::getEdgeById( EdgeId e ) const
{
    return *getEdgePtrById( e );
}

template < typename NetworkModel >
std::vector<EdgeId> 
QKD_Topology<NetworkModel>::getAdjEdgeIds( VertexId v ) const
{
    std::vector<EdgeId> adj_edges {};
    for ( const auto& [edge_id, edge_ptr] : mmEdgeToId )
        if ( edge_ptr->hasVertex( v ) )
            adj_edges.push_back( edge_id );
    return adj_edges;
}

template <typename NetworkModel>
std::vector<VertexId> QKD_Topology<NetworkModel>::getTopologyData() const
{
    std::vector<VertexId> vert_ids {};
    for ( const auto& [v_id, v_ptr] : mmVertexToId )
        vert_ids.push_back( v_id );
    return vert_ids;
}

template <typename NetworkModel>
std::ostream& operator<< ( std::ostream& os, 
                           const QKD_Topology<NetworkModel>& t )
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

#endif  // QKDTOPOLOGY_HPP
