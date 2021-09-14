#include "qkdnode.hpp"

QKD_Node::QKD_Node ( Vertex& v, std::string lbl )
:
    mVertex { v }
{
    id = last_node_id; 
    ++ last_node_id; 
}

Vertex& QKD_Node::getVertex()
{
    return mVertex;
}

bool QKD_Node::operator== ( const QKD_Node& n2 )
{
    return ( id == n2.id ) && 
           ( mVertex == n2.mVertex ) && 
           ( label == n2.label ); 
}
