#include "qkdnode.hpp"

QKD_Node::QKD_Node ( std::shared_ptr<Vertex> pv, const std::string& lbl )
:
    mpVertex { pv },
    label { lbl }
{
    id = last_node_id; 
    ++ last_node_id; 
}

NodeId QKD_Node::getNodeId()
{
    return id;
}

NodeId QKD_Node::getLastNodeId()
{
    return last_node_id;
}

Vertex& QKD_Node::getVertex()
{
    return *mpVertex;
}

bool QKD_Node::operator== ( const QKD_Node& n2 )
{
    return id == n2.id; 
}
