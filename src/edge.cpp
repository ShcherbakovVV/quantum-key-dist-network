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

bool Edge::hasVertex( Vertex& v )
{
    return v == first || v == second; 
}

bool Edge::hasVertices( Vertex& v1, Vertex& v2 )
{
    return hasVertex( v1 ) && hasVertex( v2 );
}

void Edge::reverse()
{
    std::swap( first, second ); 
}

bool Edge::operator== ( const Edge& e2 )
{
    return hasVertices( e2.first, e2.second );
}  

std::ostream& operator<< ( std::ostream& os, const Edge& e )
{
    return os << '{' << e.first << ", " << e.second << '}';
}
