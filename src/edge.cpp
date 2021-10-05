#include "edge.hpp"

Edge::Edge ( Vertex& v1, Vertex& v2 ) :
    first { v1 },
    second { v2 } 
{
    ++ first.num_adj_edges;
    ++ second.num_adj_edges;
    id = last_edge_id; 
    ++ last_edge_id; 
}

Edge::~Edge ()
{
    -- first.num_adj_edges;
    -- second.num_adj_edges;
}

EdgeId Edge::getEdgeId()
{
    return id;
}

EdgeId Edge::getLastEdgeId()
{
    return last_edge_id;
}

symmetric_pair<VertexId> Edge::getAdjVertexIds()
{
    return symmetric_pair { first.id, second.id };
}

VertexId Edge::getOtherVertexId( VertexId v )
{
    if ( v == first.getVertexId() )
        return second.getVertexId();
    else if ( v == second.getVertexId() )
        return first.getVertexId();
    throw std::out_of_range { "No vertex for given id" };
}

bool Edge::hasVertex( VertexId v )
{
    return v == first.getVertexId() || 
           v == second.getVertexId(); 
}

bool Edge::hasVertices( VertexId v1, VertexId v2 )
{
    return hasVertex( v1 ) && hasVertex( v2 );
}

bool Edge::operator== ( const Edge& e2 )
{
    return id == e2.id;
}  

std::ostream& operator<< ( std::ostream& os, const Edge& e )
{
    return os << '{' << e.first << ", " << e.second << '}';
}
