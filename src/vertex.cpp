#include "vertex.hpp"

Vertex::Vertex ()
{
    id = last_vertex_id; 
    ++ last_vertex_id; 
}

VertexId Vertex::getVertexId()
{
    return id;
}

VertexId Vertex::getLastVertexId()
{
    return last_vertex_id;
}

bool Vertex::operator== ( const Vertex& v2 )
{
    // оператор != автоматически определяется компилятором
    return id == v2.id;
}

std::ostream& operator<< ( std::ostream& os, const Vertex& v )
{
    return os << v.id;   
}
