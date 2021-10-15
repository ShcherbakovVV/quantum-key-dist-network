#include "qkdlink.hpp"

QKD_Link::QKD_Link ( const std::shared_ptr<Edge>& pe, dclr::Metrics m )
:
    mpEdge { pe },
    mMetrics { m }
{
    id = last_link_id; 
    ++ last_link_id;
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

QKD_Link::~QKD_Link ()
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed " << *this;
}

LinkId QKD_Link::getLinkId() const
{
    return id;
}

LinkId QKD_Link::getLastLinkId()
{
    return last_link_id;
}

dclr::Metrics QKD_Link::getMetricsValue() const
{
    return mMetrics;
}

void QKD_Link::setMetricsValue( dclr::Metrics m )
{
    mMetrics = m;
    BOOST_LOG_TRIVIAL(trace) << "QKD_Link: Set Metrics value " 
        << m << " on " << *this;
}

void QKD_Link::updateMetricsValue( dclr::Metrics m )
{
    setMetricsValue( getMetricsValue() + m );
}

Edge& QKD_Link::getEdge() const
{
    return *mpEdge; 
}

EdgeId QKD_Link::getEdgeId() const
{
    return mpEdge->getEdgeId(); 
}

bool QKD_Link::operator== ( const QKD_Link& l2 ) const
{
    return id == l2.id; 
}

std::ostream& operator<< ( std::ostream& os, const QKD_Link& l )
{
    return os << "Link " << l.id << " on " << l.getEdge();
}
