#include "qkdtopology.hpp"

// class Vertex
Vertex::Vertex ()
{
    id = last_vertex_id; 
    ++ last_vertex_id; 
}

Vertex::Vertex ( common::Id id )
:
    id {-1}
{}

bool Vertex::operator== ( const Vertex& v2 )
{
    // оператор != автоматически определяется компилятором
    return id == v2.id;
}

// class Edge
Edge::Edge ( Vertex& v1, Vertex& v2 ) :
    first { v1 },
    second { v2 } 
{
    id = last_edge_id; 
    ++ last_edge_id; 
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

// class QKD_Topology
Vertex& QKD_Topology::addVertex()
{
    Vertex* pv = new Vertex {};
	maVertexList.emplace_back( pv );
    mQKD_Network.mRequestGen.updMaxVertexId();
    return *pv;
}

Edge& QKD_Topology::addEdge( Vertex& v1, Vertex& v2 )
{
	Edge* pe = new Edge { v1, v2 };
	maEdgeList.emplace_back( pe );
    mQKD_Network.mRequestGen.updMaxVertexId();
	return *pe; 
}

QKD_Topology& QKD_Topology::removeVertex( Vertex& v )
{
    for ( auto p = maVertexList.begin(); p != maVertexList.end(); ++p )
    {
        // разыменование указателя (итератора) на shared_ptr
        if ( *(*p) == v )
        {
            maRemovedVertexIds.push_back( (*p)->id );
            maVertexList.erase(p);
            break;
        }
    } 

    for ( auto p = maEdgeList.begin(); p != maEdgeList.end(); ++p )
    {
        if ( (*p)->hasVertex(v) ) 
        {    
            maRemovedEdgeIds.push_back( (*p)->id );
            maEdgeList.erase(p);
        }
    } 

    return *this;
}

QKD_Topology& QKD_Topology::removeEdge( Edge& e )
{
    for ( auto p = maEdgeList.begin(); p != maEdgeList.end(); ++p )
    {
        if ( *(*p) == e ) 
        {
            maEdgeList.erase(p);
            maRemovedEdgeIds.push_back( (*p)->id );
            break;
        }
    } 
    return *this;
}

Vertex& getVertexById( common::Id id )
{
    for ( const auto& x : maVertexList )
    {
        if ( x->id == id)
            return *x;
    }
    return Vertex {-1};  // id не найден
}

// операторы вывода
std::ostream& operator<< ( std::ostream& os, const Vertex& v )
{
    return os << v.id;   
}

std::ostream& operator<< ( std::ostream& os, const Edge& e )
{
    return os << '{' << e.first << ", " << e.second << '}';
}

std::ostream& operator<< ( std::ostream& os, const QKD_Topology& t )
{
    os << "Vertices: ";
    for ( const auto& p : t.maVertexList )
        os << *p << ", ";

    os << '\n';

    os << "Edges: ";
    for ( const auto& p : t.maEdgeList )
        os << *p << ", ";

    return os;
}
