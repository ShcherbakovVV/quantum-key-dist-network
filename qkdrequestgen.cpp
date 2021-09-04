#include "qkdrequestgen.hpp"

#include "qkdnetwork.hpp"
#include "qkdtopology.hpp"

Request::Request ( TimePoint gen, TimePoint exp, Vertex& s, Vertex& d ) 
:
	gen_time { gen }, 
	exp_time { exp },  
    start { s },
    dest { d }
{}

QKD_RequestGen::QKD_RequestGen ( QKD_Network& parent, Duration age = 60000ms ) 
:
    mQKD_Network { parent },
    mRequestAge { age },
    mMaxVertexId { Vertex::last_vertex_id },
    mIntRNG { 0, mMaxVertexId }
{}
 
common::Id genRandomId()
{
    bool b = true;
    while (b)
    {
        b = false;  // отключаем итерацию
        // static_cast на случай common::Id не являющегося int
        common::Id res = static_cast< common::Id >( mIntRNG() ); 
        for ( auto& p = mQKD_Network.mTopology.maRemovedVertexIds.begin(); 
              p != mQKD_Network.mTopology.maRemovedVertexIds.end(); 
              ++ p )
        {
            if ( *p == res )
                b = true; // сгенерированный id не подходит, итерируемся дальше
        }
    }
    return res;
}

const Request QKD_RequestGen::genRequest()
{
    Vertex& start = mQKD_Network.mTopology.getVertexById( genRandomId() );     
    Vertex& dest = mQKD_Network.mTopology.getVertexById( genRandomId() );     
    TimePoint now = mQKD_Network.mClock::now();
    return Request { now, now + mRequestAge, start, dest };   
}

void QKD_RequestGen::setRequestAge( Duration age )
{
    mRequestAge = age; 
}

void QKD_RequestGen::updMaxVertexId()
{
    mMaxVertexId = Vertex::last_vertex_id;
}
