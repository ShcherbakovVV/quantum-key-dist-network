#include "edge.hpp"

Edge::Edge ( Vertex& v1, Vertex& v2 ) 
:
    mpVertex1 { &v1 },
    mpVertex2 { &v2 } 
{
    ++ mpVertex1->num_adj_edges;
    ++ mpVertex2->num_adj_edges;
    id = last_edge_id; 
    ++ last_edge_id;
    BOOST_LOG_TRIVIAL(trace) << "Added " << *this;
}

Edge::~Edge ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destroyed " << *this;
    -- mpVertex1->num_adj_edges;
    -- mpVertex2->num_adj_edges;
}

EdgeId Edge::getEdgeId() const
{
    return id;
}

EdgeId Edge::getLastEdgeId()
{
    return last_edge_id;
}

symmetric_pair<VertexId> Edge::getAdjVertexIds() const
{
    return symmetric_pair { mpVertex1->id, mpVertex2->id };
}

VertexId Edge::getOtherVertexId( VertexId v ) const
{
    if ( v == mpVertex1->getVertexId() )
        return mpVertex2->getVertexId();
    else if ( v == mpVertex2->getVertexId() )
        return mpVertex1->getVertexId();
    throw std::out_of_range { "No vertex for given id" };
}

bool Edge::hasVertex( VertexId v ) const
{
    return v == mpVertex1->getVertexId() || 
           v == mpVertex2->getVertexId(); 
}

bool Edge::hasVertices( VertexId v1, VertexId v2 ) const
{
    return hasVertex( v1 ) && hasVertex( v2 );
}

bool Edge::operator== ( const Edge& e2 ) const
{
    return id == e2.id;
}  

std::ostream& operator<< ( std::ostream& os, const Edge& e )
{
    return os << "Edge " << e.id 
        << " {" << *e.mpVertex1 << ", " << *e.mpVertex2 << '}';
}
