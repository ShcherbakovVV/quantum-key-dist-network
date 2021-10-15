#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <iostream>

#include "id.hpp"
#include "common.hpp"
#include "vertex.hpp"
#include "symmetric_pair.hpp"

class Edge;

using EdgeId = Id<Edge, dclr::IdRep>;

class Edge
{
    template <typename NetworkModel>
        friend class QKD_Topology;
    friend std::ostream& operator<< ( std::ostream&, const Edge& );

    using id_type = EdgeId;

    private:

        EdgeId id;
        static inline EdgeId last_edge_id = 0;
        
        Vertex* mpVertex1;
        Vertex* mpVertex2;

        Edge () = delete;
        explicit Edge ( Vertex&, Vertex& );
        
    public:
        
        ~Edge();
        
        EdgeId getEdgeId() const;
        static EdgeId getLastEdgeId();
        
        symmetric_pair<VertexId> getAdjVertexIds() const;
        VertexId getOtherVertexId( VertexId ) const;
        
        bool hasVertex( VertexId ) const;
        bool hasVertices( VertexId, VertexId ) const;

        bool operator== ( const Edge& ) const;
};

std::ostream& operator<< ( std::ostream&, const Edge& );

#endif  // EDGE_HPP
