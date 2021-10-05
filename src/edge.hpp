#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <iostream>

#include "id.hpp"
#include "common.hpp"
#include "vertex.hpp"
#include "symmetric_pair.hpp"

class Edge;

using EdgeId = Id< Edge, dclr::IdRep >;
//static EdgeId EDGE_INVALID {-1};

class Edge
{
    template <typename NetworkModel>
        friend class QKD_Topology;
    friend std::ostream& operator<< ( std::ostream&, const Edge& );

    using id_type = EdgeId;

    private:

        EdgeId id;
        static inline EdgeId last_edge_id = 0;
        
        Vertex& first;
        Vertex& second;

        Edge () = delete;
        Edge ( Vertex&, Vertex& );
        
    public:
        
        ~Edge();
        
        EdgeId getEdgeId();
        static EdgeId getLastEdgeId();
        
        symmetric_pair<VertexId> getAdjVertexIds();
        VertexId getOtherVertexId( VertexId );
        
        bool hasVertex( VertexId );
        bool hasVertices( VertexId, VertexId );

        bool operator== ( const Edge& );
};

std::ostream& operator<< ( std::ostream&, const Edge& );

#endif  // EDGE_HPP
