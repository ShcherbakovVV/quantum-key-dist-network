#ifndef QKDLINK_HPP
#define QKDLINK_HPP

#include <memory>
#include <concepts>

#include <boost/log/trivial.hpp>

#include "lib/id.hpp"

#include "edge.hpp"
#include "qkdnode.hpp"

template <std::unsigned_integral IdRep, typename Metrics>
class QKD_LinkT
{
public:

    template <ClockType Clk, DurationType Dur,
              std::uniform_random_bit_generator Eng, typename Dist,
              typename Mtr, std::unsigned_integral Int>
    friend class QKD_Network;

    using this_type = QKD_LinkT<IdRep, Metrics>;
    using id_type   = Id<this_type, IdRep>;

    using Edge = EdgeT<IdRep>;

    using EdgeId  = typename EdgeT<IdRep>::id_type;
    using LinkId  = id_type;

private:

    LinkId id;
    static inline LinkId last_id = 0;

    Metrics mMetrics;

    std::shared_ptr<Edge> mpEdge;

    QKD_LinkT () = delete;
    QKD_LinkT ( const std::shared_ptr<Edge>&, Metrics );

public:

    ~QKD_LinkT ();

    LinkId getLinkId() const        { return id; }
    static LinkId getLastLinkId()   { return last_id; }

    Metrics getMetricsValue() const { return mMetrics; }
    void setMetricsValue( Metrics );
    void updateMetricsValue( Metrics );

    Edge& getEdge() const           { return *mpEdge; }
    EdgeId getEdgeId() const        { return mpEdge->getEdgeId(); }

    bool operator== ( const this_type& l2 ) const { return id == l2.id; };

    // !!! сортируется не по id, а по метрике !!!
    //bool operator<  ( const this_type& l2 ) const
        //{ return mMetrics < l2.mMetrics; };
    bool operator<  ( const this_type& l2 ) const { return id < l2.id; };
};

template <std::unsigned_integral IdRep, typename Metrics>
QKD_LinkT<IdRep, Metrics>::QKD_LinkT ( const std::shared_ptr<Edge>& pe,
                                       Metrics m )
:
    mpEdge { pe },
    mMetrics { m }
{
    ++ last_id;
    id = last_id;
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

template <std::unsigned_integral IdRep, typename Metrics>
QKD_LinkT<IdRep, Metrics>::~QKD_LinkT ()
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

template <std::unsigned_integral IdRep, typename Metrics>
void QKD_LinkT<IdRep, Metrics>::setMetricsValue( Metrics m )
{
    mMetrics = m;
    BOOST_LOG_TRIVIAL(info) << "QKD_Link: Set Metrics value "
        << m << " on " << *this;
}

template <std::unsigned_integral IdRep, typename Metrics>
void QKD_LinkT<IdRep, Metrics>::updateMetricsValue( Metrics m )
{
    setMetricsValue( getMetricsValue()+m );
}

template <std::unsigned_integral IdRep, typename Metrics>
std::ostream& operator<< ( std::ostream& os,
                           const QKD_LinkT<IdRep, Metrics>& l )
{
    return os << "Link " << l.getLinkId() << " on " << l.getEdge();
}

#endif  // QKDLINK_HPP
