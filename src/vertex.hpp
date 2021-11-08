#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <concepts>
#include <iostream>

#include <boost/log/trivial.hpp>

#include "lib/id.hpp"

template <std::unsigned_integral IdRep>
class VertexT
{
public:

    template <std::unsigned_integral Rep> friend class EdgeT;
    template <typename Network>           friend class QKD_Topology;

    using this_type = VertexT<IdRep>;
    using id_type   = Id<this_type, IdRep>;

    using VertexId = id_type;

private:

    VertexId id;
    static inline VertexId last_id { 0 };
        // inline, т.к. запрещена инициализация static-полей

    int num_adj_edges = 0;

    VertexT ();

public:

    ~VertexT ();

    VertexId getVertexId() const      { return id; }
    static VertexId getLastVertexId() { return last_id; }

    bool operator== ( const this_type& v2 ) const { return id == v2.id; }
    bool operator<  ( const this_type& v2 ) const { return id < v2.id; }
};

template <std::unsigned_integral IdRep>
VertexT<IdRep>::VertexT ()
{
    ++ last_id;
    id = last_id;
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

template <std::unsigned_integral IdRep>
VertexT<IdRep>::~VertexT ()
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

template <std::unsigned_integral IdRep>
std::ostream& operator<< ( std::ostream& os, const VertexT<IdRep>& v )
{
    return os << "Vertex " << v.getVertexId();
}

#endif  // VERTEX_HPP
