#include "qkdnode.hpp"

QKD_Node::QKD_Node (const std::shared_ptr<Vertex>& pv, const std::string& lbl)
:
    mpVertex { pv },
    label { lbl }
{
    id = last_node_id; 
    ++ last_node_id;
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

QKD_Node::~QKD_Node ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed " << *this;
}

NodeId QKD_Node::getNodeId() const
{
    return id;
}

NodeId QKD_Node::getLastNodeId()
{
    return last_node_id;
}

Vertex& QKD_Node::getVertex() const
{
    return *mpVertex;
}

VertexId QKD_Node::getVertexId() const
{
    return mpVertex->getVertexId();
}

bool QKD_Node::operator== ( const QKD_Node& n2 ) const
{
    return id == n2.id; 
}

std::ostream& operator<< ( std::ostream& os, const QKD_Node& n )
{
    return os << "Node " << n.id << " on " << n.getVertex();
}
