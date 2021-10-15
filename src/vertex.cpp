#include "vertex.hpp"

Vertex::Vertex ()
{
    id = last_vertex_id; 
    ++ last_vertex_id;
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

Vertex::~Vertex ()
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

VertexId Vertex::getVertexId() const
{
    return id;
}

VertexId Vertex::getLastVertexId()
{
    return last_vertex_id;
}

bool Vertex::operator== ( const Vertex& v2 ) const
{
    return id == v2.id;
}

std::ostream& operator<< ( std::ostream& os, const Vertex& v )
{
    return os << "Vertex " << v.id;   
}
