#ifndef EDGE_HPP
#define EDGE_HPP

#include <concepts>
#include <iostream>

#include "lib/id.hpp"
#include "lib/symmetric_pair.hpp"

#include "vertex.hpp"

template <std::unsigned_integral IdRep>
class EdgeT
{
public:

    template <typename Network> friend class QKD_Topology;

    using this_type = EdgeT<IdRep>;
    using id_type   = Id<this_type, IdRep>;

    using Vertex = VertexT<IdRep>;

    using EdgeId   = id_type;
    using VertexId = typename Vertex::id_type;

private:

    EdgeId id;
    static inline EdgeId last_id = 0;

    Vertex* mpVertex1;
    Vertex* mpVertex2;

    EdgeT () = delete;
    EdgeT ( Vertex&, Vertex& );

public:

    ~EdgeT();

    EdgeId getEdgeId() const      { return id; }
    static EdgeId getLastEdgeId() { return last_id; }

    symmetric_pair<Vertex*> getAdjVertexPtrs() const
        { return symmetric_pair { mpVertex1, mpVertex2 }; }

    symmetric_pair<VertexId> getAdjVertexIds() const
        { return symmetric_pair { mpVertex1->id, mpVertex2->id }; }

    VertexId getOtherVertexId( VertexId ) const;

    bool hasVertex( VertexId ) const;
    bool hasVertices( VertexId, VertexId ) const;

    bool operator== ( const this_type& e2 ) const { return id == e2.id; }
    bool operator<  ( const this_type& e2 ) const { return id < e2.id; }
};

template <std::unsigned_integral IdRep>
EdgeT<IdRep>::EdgeT ( Vertex& v1, Vertex& v2 )
:
    mpVertex1 { &v1 },
    mpVertex2 { &v2 }
{
    ++ mpVertex1->num_adj_edges;
    ++ mpVertex2->num_adj_edges;
    ++ last_id;
    id = last_id;
    BOOST_LOG_TRIVIAL(trace) << "Added " << *this;
}

template <std::unsigned_integral IdRep>
EdgeT<IdRep>::~EdgeT ()
{
    -- mpVertex1->num_adj_edges;
    -- mpVertex2->num_adj_edges;
    BOOST_LOG_TRIVIAL(trace) << "Destroyed " << *this;
}

template <std::unsigned_integral IdRep>
typename EdgeT<IdRep>::VertexId
EdgeT<IdRep>::getOtherVertexId( VertexId v ) const
{
    if ( v == mpVertex1->getVertexId() )
        return mpVertex2->getVertexId();
    else if ( v == mpVertex2->getVertexId() )
        return mpVertex1->getVertexId();
    throw std::out_of_range { "No Vertex for given id" };
}

template <std::unsigned_integral IdRep>
bool EdgeT<IdRep>::hasVertex( VertexId v ) const
{
    return v == mpVertex1->getVertexId() ||
           v == mpVertex2->getVertexId();
}

template <std::unsigned_integral IdRep>
bool EdgeT<IdRep>::hasVertices( VertexId v1, VertexId v2 ) const
{
    return hasVertex( v1 ) && hasVertex( v2 );
}

template <std::unsigned_integral Rep>
std::ostream& operator<< ( std::ostream& os, const EdgeT<Rep>& e )
{
    symmetric_pair<typename EdgeT<Rep>::Vertex*> pair = e.getAdjVertexPtrs();
    return os << "Edge " << e.getEdgeId()
        << " {" << *pair.first << ", " << *pair.second << '}';
}

#endif  // EDGE_HPP
