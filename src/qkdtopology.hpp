#ifndef QKDTOPOLOGY_HPP
#define QKDTOPOLOGY_HPP

#include <map>
#include <vector> 
#include <memory> 
#include <iostream>
#include <stdexcept>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp"
#include "qkdrequestgen.hpp"

template < typename NetworkModel >
class QKD_Topology
{
    friend class Vertex;
    friend class Edge;

    template < typename Network >
    friend std::ostream& operator<< (std::ostream& os, 
                                     const QKD_Topology< Network >& t);

    private:

        const NetworkModel& mQKD_Network;

        std::map< VertexId, std::shared_ptr<Vertex> > mmVertexToId;    
        std::map< EdgeId, std::shared_ptr<Edge> > mmEdgeToId;

        std::vector< VertexId > maRemovedVertexIdList;
        std::vector< EdgeId > maRemovedEdgeIdList;

    public:
        
        QKD_Topology () = delete;
        QKD_Topology ( NetworkModel& );

        VertexId addVertex();
        EdgeId addEdge( VertexId, VertexId );

        void removeVertex( VertexId );
        void removeEdge( EdgeId );

        Vertex& getVertexById( VertexId );
        Edge& getEdgeById( EdgeId );

};

template < typename NetworkModel >
QKD_Topology< NetworkModel >::QKD_Topology ( NetworkModel& parent )
:
    mQKD_Network { parent }
{}

template < typename NetworkModel >
VertexId QKD_Topology< NetworkModel >::addVertex()
{
    Vertex* pv = new Vertex {};
    mmVertexToId.emplace( pv->id, pv );
    mQKD_Network.mRequestGen.updMaxVertexId();
    return pv->id;
}

template < typename NetworkModel >
EdgeId QKD_Topology< NetworkModel >::addEdge( VertexId v1, VertexId v2 )
{
    Edge* pe = new Edge { getVertexById( v1 ), getVertexById( v2 ) };
    mmEdgeToId.emplace( pe->id, pe );
    mQKD_Network.mRequestGen.updMaxVertexId();
    return pe->id; 
}

template < typename NetworkModel >
void QKD_Topology< NetworkModel >::removeVertex( VertexId v )
{
    for ( auto pe = mmEdgeToId.begin(); pe != mmEdgeToId.end(); ++pe )
        if ( pe->second->hasVertex( getVertexById( v ) ) ) 
        {
            maRemovedEdgeIdList.push_back( pe->first );
            mmEdgeToId.erase( pe );
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
void QKD_Topology< NetworkModel >::removeEdge( EdgeId e )
{
    for ( auto p = mmEdgeToId.begin(); p != mmEdgeToId.end(); ++ p )
        if ( p->first == e ) 
        {
            -- p->second->first.num_adj_edges; 
            -- p->second->second.num_adj_edges; 
            maRemovedEdgeIdList.push_back( p->first );
            mmEdgeToId.erase( p );
            break;
        }
}

template < typename NetworkModel >
Vertex& QKD_Topology< NetworkModel >::getVertexById( VertexId id )
{
    if ( id != VERTEX_INVALID )
        return *mmVertexToId.at( id );
    throw std::out_of_range( "Invalid vertex id" );
}

template < typename NetworkModel >
Edge& QKD_Topology< NetworkModel >::getEdgeById( EdgeId id )
{
    if ( id != EDGE_INVALID )
        return *mmEdgeToId.at( id );
    throw std::out_of_range( "Invalid edge id" );
}

template < typename NetworkModel >
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

#endif  // QKDTOPOLOGY_HPP
