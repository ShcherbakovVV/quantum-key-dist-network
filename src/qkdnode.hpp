#ifndef QKDNODE_HPP
#define QKDNODE_HPP

#include <memory>
#include <string>
#include <concepts>

#include <boost/log/trivial.hpp>

#include "lib/id.hpp"

#include "vertex.hpp"

template <std::unsigned_integral IdRep>
class QKD_NodeT
{
public:

    template <ClockType Clk, DurationType Dur,
              std::uniform_random_bit_generator Eng, typename Dist,
              typename Mtr, std::unsigned_integral Int>
    friend class QKD_Network;

    using this_type = QKD_NodeT<IdRep>;
    using id_type = Id<this_type, IdRep>;

    using Vertex = VertexT<IdRep>;

    using VertexId = typename Vertex::id_type;
    using NodeId   = id_type;

private:

    NodeId id;
    static inline NodeId last_id = 0;

    std::shared_ptr<Vertex> mpVertex;

    QKD_NodeT () = delete;
    // а если строка - временный объект?
    QKD_NodeT ( const std::shared_ptr<Vertex>&, std::string );

public:

    ~QKD_NodeT ();

    std::string label;

    NodeId getNodeId() const      { return id; }
    static NodeId getLastNodeId() { return last_id; }

    Vertex& getVertex() const     { return *mpVertex; }
    VertexId getVertexId() const  { return mpVertex->getVertexId(); }

    bool operator== ( const this_type& n2 ) const { return id == n2.id; }
    bool operator<  ( const this_type& n2 ) const { return id < n2.id; }
};

template <std::unsigned_integral IdRep>
QKD_NodeT<IdRep>::QKD_NodeT ( const std::shared_ptr<Vertex>& pv,
                              std::string lbl )
:
    mpVertex { pv },
    label    { lbl }
{
    ++ last_id;
    id = last_id;
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

template <std::unsigned_integral IdRep>
QKD_NodeT<IdRep>::~QKD_NodeT ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed " << *this;
}

template <std::unsigned_integral IdRep>
std::ostream& operator<< ( std::ostream& os, const QKD_NodeT<IdRep>& n )
{
    return os << "Node " << n.getNodeId() << " on " << n.getVertex();
}

#endif  // QKDNODE_HPP
