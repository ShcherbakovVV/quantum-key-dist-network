#ifndef QKDREQUESTGEN_HPP
#define QKDREQUESTGEN_HPP

#include <chrono>
#include <random>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp" 
#include "qkdtopology.hpp"

using std::chrono_literals::operator""ms;

template < typename Clock >
struct RequestType
{
    using TimePoint = typename std::chrono::time_point< Clock >;

    TimePoint gen_time;  
    TimePoint exp_time;  
    VertexId start; 
    VertexId dest; 

    RequestType () = delete;

    private:
        
        RequestType ( TimePoint, TimePoint, VertexId, VertexId ); 
};

template < typename Clock >
RequestType< Clock >::RequestType
    ( TimePoint gen, TimePoint exp, VertexId s, VertexId d ) 
:
    gen_time { gen }, 
    exp_time { exp },  
    start { s },
    dest { d }
{}

template < typename NetworkModel,
           typename IntRNG = std::uniform_int_distribution< int > >
class QKD_RequestGen
{ 
    public:

        template < typename Clock >
            friend class RequestType;

        using Request = RequestType< dclr::SysClock >;

    private:
    
        const NetworkModel& mQKD_Network;

        IntRNG mIntRNG;
        dclr::Duration mRequestAge;  // время жизни заявки - 1 минута
        VertexId mMaxVertexId;
    
        VertexId genRandomVertexId(); 

    public:
    
        QKD_RequestGen () = delete;
        QKD_RequestGen ( NetworkModel&, dclr::Duration age = 60000ms );
                                           
        const Request genRequest();
        void setRequestAge( dclr::Duration );
        void updMaxVertexId();
};

template < typename NetworkModel, typename IntRNG >
QKD_RequestGen< NetworkModel, IntRNG >::QKD_RequestGen
    ( NetworkModel& parent, dclr::Duration age ) 
:
    mQKD_Network { parent },
    mRequestAge { age },
    mMaxVertexId { Vertex::last_vertex_id.value },
    mIntRNG { 0, Vertex::last_vertex_id.value }
{}
 
template < typename NetworkModel, typename IntRNG >
VertexId QKD_RequestGen< NetworkModel, IntRNG >::genRandomVertexId()
{
    VertexId res { mIntRNG() };
    do
    {
        
        for ( auto p = mQKD_Network.mTopology.maRemovedVertexIdList.begin(); 
              p != mQKD_Network.mTopology.maRemovedVertexIdList.end(); 
              ++ p )
        {
            if ( *p == res )
                break;
            res = mIntRNG();
        }
    }
    while (true);
    return res;
}

template < typename NetworkModel, typename IntRNG >
const typename QKD_RequestGen< NetworkModel, IntRNG >::Request 
QKD_RequestGen< NetworkModel, IntRNG >::genRequest()
{
    VertexId start = genRandomVertexId();     
    VertexId dest = genRandomVertexId();
    typename QKD_RequestGen< NetworkModel, IntRNG >::Request::TimePoint now =
        mQKD_Network.now();
    return Request { now, now + mRequestAge, start, dest };   
}

template < typename NetworkModel, typename IntRNG >
void QKD_RequestGen<NetworkModel, IntRNG>::setRequestAge( dclr::Duration age )
{
    mRequestAge = age; 
}

template < typename NetworkModel, typename IntRNG >
void QKD_RequestGen< NetworkModel, IntRNG >::updMaxVertexId()
{
    mMaxVertexId = Vertex::last_vertex_id.value;
}

#endif  // QKDREQUESTGEN_HPP
