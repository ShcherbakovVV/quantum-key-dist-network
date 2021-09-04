#ifndef QKDTOPOLOGY_HPP
#define QKDTOPOLOGY_HPP

#include <vector> 
#include <memory> 
#include <utility> 
#include <iostream>
#include <algorithm>

#include "common.hpp"
#include "qkdnetwork.hpp"
#include "qkdrequestgen.hpp"

// что делать, если вершина осталась без ребер?
class Vertex
{
    friend std::ostream& operator<< ( std::ostream& os, const Vertex& v );

	private:

        common::Id id;

        Vertex ( common::Id id );  // для возвращения вектора с id = -1
                                   // (сигнализирует "не найдено" при 
                                   // использовании getVertexById() )

	public:

        static inline common::Id last_vertex_id = 0; // inline, т.к. запрещена
                                                  // инициализация static-полей
        bool operator== ( const Vertex& v2 );
};

class Edge 
{
    friend std::ostream& operator<< ( std::ostream& os, const Edge& e );

	private:

        common::Id id;
        // правильное удаление производят deleteEdge() и deleteVertex()
        Vertex& first;
        Vertex& second;

        Edge () = delete;
        Edge ( Vertex& v1, Vertex& v2 );
        
    public:

        static inline common::Id last_edge_id = 0;
        
        bool hasVertex( Vertex& v );
        bool hasVertices( Vertex& v1, Vertex& v2 );

        void reverse();

        bool operator== ( const Edge& e2 );
};

class QKD_Topology
{
	friend class Vertex;
	friend class Edge;

    friend std::ostream& operator<< (std::ostream& os, const QKD_Topology& t);

	private:

        QKD_Network& mQKD_Network;

        std::vector< std::shared_ptr< Vertex > > maVertexList;	
        std::vector< std::shared_ptr< Edge > > maEdgeList;

        std::vector< common::Id > maRemovedVertexIds;
        std::vector< common::Id > maRemovedEdgeIds;

	public:
			
        QKD_Topology ( QKD_Network& parent );

        Vertex& addVertex();
        Edge& addEdge( Vertex& v1, Vertex& v2 );

        void removeVertex( Vertex& v );
        void removeEdge( Edge& e );

        Vertex& getVertexById( common::Id id );
        //Edge& getEdgeById(common::Id id);
};

// операторы вывода
std::ostream& operator<< ( std::ostream& os, const Vertex& v );
std::ostream& operator<< ( std::ostream& os, const Edge& e );
std::ostream& operator<< ( std::ostream& os, const QKD_Topology& t );

#endif  // QKDTOPOLOGY_HPP
