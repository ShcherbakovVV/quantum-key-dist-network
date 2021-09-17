#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <iostream>

#include "common.hpp"
#include "vertex.hpp"

class Edge;

using EdgeId = tools::Id< Edge, dclr::IdRep >;
EdgeId EDGE_INVALID {-1};

class Edge
{
    friend std::ostream& operator<< ( std::ostream&, const Edge& );

    private:

        EdgeId id;

        Vertex& first;
        Vertex& second;

        Edge () = delete;
        Edge ( Vertex&, Vertex& );

        ~Edge();
        
    public:

        static inline EdgeId last_edge_id = 0;
        
        bool hasVertex( Vertex& );
        bool hasVertices( Vertex&, Vertex& );

        void reverse();

        bool operator== ( const Edge& );
};

std::ostream& operator<< ( std::ostream&, const Edge& );

#endif  // EDGE_HPP
