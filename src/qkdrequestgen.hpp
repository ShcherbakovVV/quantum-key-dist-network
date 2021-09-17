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
    
        NetworkModel& mQKD_Network;

        IntRNG& mIntRNG;
        dclr::Duration mRequestAge;  // время жизни заявки - 1 минута
        VertexId mMaxVertexId;
    
        VertexId genRandomVertexId(); 

    public:
    
        QKD_RequestGen ( NetworkModel&, dclr::Duration );
                                           
        const Request genRequest();
        void setRequestAge( dclr::Duration );
        void updMaxVertexId();
};

template < typename NetworkModel >
QKD_RequestGen< NetworkModel >::QKD_RequestGen
    ( NetworkModel& parent, dclr::Duration age = 60000ms ) 
:
    mQKD_Network { parent },
    mRequestAge { age },
    mMaxVertexId { Vertex::last_vertex_id },
    mIntRNG { 0, Vertex::last_vertex_id }
{}
 
template < typename NetworkModel >
VertexId QKD_RequestGen< NetworkModel >::genRandomVertexId()
{
    bool b = true;
    while (b)
    {
        b = false;  // отключаем итерацию
        // static_cast на случай dclr::Id не являющегося int
        VertexId res = static_cast< VertexId >( mIntRNG() ); 
        for ( auto& p = mQKD_Network.mTopology.maRemovedVertexIdList.begin(); 
              p != mQKD_Network.mTopology.maRemovedVertexIdList.end(); 
              ++ p )
        {
            if ( *p == res )
                b = true; //сгенерированный id не подходит, итерируемся дальше
        }
    }
    return res;
}

template < typename NetworkModel >
const typename QKD_RequestGen< NetworkModel >::Request 
QKD_RequestGen< NetworkModel >::genRequest()
{
    VertexId start = genRandomVertexId();     
    VertexId dest = genRandomVertexId();
    TimePoint now = mQKD_Network.mClock::now();
    return Request { now, now + mRequestAge, start, dest };   
}

template < typename NetworkModel >
void QKD_RequestGen< NetworkModel >::setRequestAge( dclr::Duration age )
{
    mRequestAge = age; 
}

template < typename NetworkModel >
void QKD_RequestGen< NetworkModel >::updMaxVertexId()
{
    mMaxVertexId = Vertex::last_vertex_id;
}

#endif  // QKDREQUESTGEN_HPP
