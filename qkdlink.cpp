#include "qkdlink.hpp"

QKD_Link::QKD_Link ( Edge& e, dclr::Metrics m )
:
    mEdge { e },
    mMetrics { m }
{
    id = last_link_id; 
    ++ last_link_id; 
}

Edge& QKD_Link::getEdge()
{
    return mEdge; 
}

bool QKD_Link::operator== ( const QKD_Link& l2 )
{
    return ( id == l2.id ) && 
           ( mEdge == l2.mEdge ) && 
           ( mMetrics == l2.mMetrics ); 
}
