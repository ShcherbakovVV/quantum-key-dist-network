#include "qkdlink.hpp"

QKD_Link::QKD_Link ( std::shared_ptr< Edge > pe, dclr::Metrics m )
:
    mpEdge { pe },
    mMetrics { m }
{
    id = last_link_id; 
    ++ last_link_id; 
}

Edge& QKD_Link::getEdge()
{
    return *mpEdge; 
}

bool QKD_Link::operator== ( const QKD_Link& l2 )
{
    return ( id == l2.id ) && 
           ( mpEdge == l2.mpEdge ) && 
           ( mMetrics == l2.mMetrics ); 
}
