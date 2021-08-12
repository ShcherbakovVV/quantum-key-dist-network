#ifndef QKDTOPOLOGY_HPP
#define QKDTOPOLOGY_HPP

#include <vector> 
#include <memory> 
#include <utility> 
#include <iostream>
#include <algorithm>

#include "common.hpp"

class Vertex
{
	private:

		// установка значения последнего id происходит 
		// при инициализации QKD_Topology 
		static INT_ID last_vertex_id; 
		const INT_ID id;

    Vertex() = default;

    std::ostream& operator<< (std::ostream& os);
};

class Edge 
{
	private:

		static INT_ID last_edge_id;
		INT_ID id;

		std::shared_ptr<Vertex> first;
		std::shared_ptr<Vertex> second;  // новая вершина всегда second
		
		Edge () = delete;
		Edge (Vertex& v1, Vertex& v2);

		bool hasVertex(Vertex& v);
		bool hasVertices(Vertex& v1, Vertex& v2);

        void reverse();

		std::ostream& operator<< (std::ostream& os);
};

class QKD_Topology
{
	friend class Vertex;
	friend class Edge;

	private:

		std::vector< std::shared_ptr<Vertex> > mVertexList;	
		std::vector< std::unique_ptr<Edge> > mEdgeList;

	public:
			
		QKD_Topology () = default;
		
        Vertex addVertex();
		Edge addEdge(Vertex& v1, Vertex& v2);

        QKD_Topology& deleteVertex(Vertex& v);
        QKD_Topology& deleteEdge(Edge& e);

        std::ostream& operator<< (std::ostream& os);
};

#endif  // QKDTOPOLOGY_HPP
