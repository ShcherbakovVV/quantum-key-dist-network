#include "qkdlink.hpp"

QKD_Link::QKD_Link ( std::shared_ptr<Edge> pe, dclr::Metrics m )
:
    mpEdge { pe },
    mMetrics { m }
{
    id = last_link_id; 
    ++ last_link_id; 
}

LinkId QKD_Link::getLinkId()
{
    return id;
}

LinkId QKD_Link::getLastLinkId()
{
    return last_link_id;
}

dclr::Metrics QKD_Link::getMetricsValue()
{
    return mMetrics;
}

void QKD_Link::setMetricsValue( dclr::Metrics m )
{
    mMetrics = m;
}

Edge& QKD_Link::getEdge()
{
    return *mpEdge; 
}

bool QKD_Link::operator== ( const QKD_Link& l2 )
{
    return id == l2.id; 
}
