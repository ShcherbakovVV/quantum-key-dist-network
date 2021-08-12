#include "qkdtopology.hpp"

// class Vertex
Vertex()
{
    id = last_vertex_id; 
    ++ last_vertex_id; 
}

std::ostream& operator<< (std::ostream& os)
{
    return os << "id: " << id;   
}

// class Edge
Edge::Edge (Vertex& v1, Vertex& v2) :
    first {&v1},
    second {&v2} 
{
    id = last_edge_id; 
    ++ last_edge_id; 
}

bool Edge::hasVertex(Vertex& v)
{
    return v == *first || v == *second; 
}

bool Edge::hasVertices(Vertex& v1, Vertex& v2)
{
    return hasVertex(v1) && hasVertex(v2);
}

void Edge::reverse()
{
    std::swap(first, second); 
}

bool Edge::operator== (const Edge& e)
{
    return e.hasVertices(Edge::first, Edge::second);
}  // опервтор != автоматически определяется компилятором

std::ostream& Edge::operator<< (std::ostream& os)
{
    return os << '{' << *first << ", " << *second << '}';
}

// class QKD_Topology
QKD_Topology::QKD_Topology ()
{
    INT_ID Vertex::last_vertex_id = 0;
	INT_ID Edge::last_edge_id = 0;
}

Vertex QKD_Topology::addVertex()
{
    Vertex v {};
	mVertexlist.push_back(std::shared_ptr<Vertex> {v});
    return v;
}

Edge QKD_Topology::addEdge(Vertex& v1, Vertex& v2)
{
	Edge e {v1, v2};
	mEdgelist.push_back(std::unique_ptr<Edge> {e});
	return e; 
}

QKD_Topology& QKD_Topology::deleteVertex(Vertex& v)
{
    for(const auto& p=mVertexlist.begin(); p!=mVertexlist.end(); ++p)
    {
        if(*p == v)
        {
            mVertexlist.erase(p);
            break;
        }
    } 
    std::remove_if(mEdgeList.begin(), 
                   mEdgeList.end(), 
                   mEdgeList.has_vertex(v));
    return *this;
}

QKD_Topology& QKD_Topology::deleteEdge(Edge& e)
{
    std::remove_if(mEdgeList.begin(), mEdgeList.end(), e); 
    return *this;
}

std::ostream& QKD_Topology::operator<< (std::ostream& os)
{
    os << "Vertices:\n";
    for(const auto& p : mVertexlist)
        os << *p << '\n';

    os << "Edges:\n;
    for(const auto& p : mEdgelist)
        os << *p << '\n';
    return os;
}
