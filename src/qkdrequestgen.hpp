#ifndef QKDREQUESTGEN_HPP
#define QKDREQUESTGEN_HPP

#include <chrono>
#include <random>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp" 
#include "qkdtopology.hpp"

using std::chrono_literals::operator""ms;

template <typename TPoint>
struct RequestType
{
    template <typename NetworkModel, typename IntRNG>
        friend class QKD_RequestGen;

    TPoint gen_time;  
    TPoint exp_time;  
    NodeId start; 
    NodeId dest; 

    RequestType () = delete;

    private:
        
        RequestType ( TPoint, TPoint, NodeId, NodeId ); 
};

template <typename TPoint>
RequestType<TPoint>::RequestType ( TPoint gen, TPoint exp, NodeId s, NodeId d ) 
:
    gen_time { gen }, 
    exp_time { exp },  
    start { s },
    dest { d }
{}

template < typename NetworkModel,
           typename IntRNG = std::uniform_int_distribution<int> >
class QKD_RequestGen
{ 
    using Request = RequestType< dclr::TimeMoment >;

    private:
    
        const NetworkModel& mQKD_Network;

        IntRNG mIntRNG;
        dclr::Duration mRequestLifetime;
        NodeId mMaxNodeId;
    
        NodeId genRandomNodeId(); 

    public:
    
        QKD_RequestGen () = delete;
        QKD_RequestGen ( NetworkModel&, dclr::Duration age = 60000ms );
                                           
        const Request genRequest();
        void setRequestLifetime( dclr::Duration );
        void updMaxNodeId();
};

template < typename NetworkModel, typename IntRNG >
QKD_RequestGen< NetworkModel, IntRNG >::QKD_RequestGen
    ( NetworkModel& parent, dclr::Duration age ) 
:
    mQKD_Network      { parent },
    mRequestLifetime  { age },
    mMaxNodeId        { QKD_Node::getLastNodeId().value },
    mIntRNG           { 0, QKD_Node::getLastNodeId().value }
{}
 
template < typename NetworkModel, typename IntRNG >
NodeId QKD_RequestGen< NetworkModel, IntRNG >::genRandomNodeId()
{
    NodeId res;
    do
    {
        res = mIntRNG();
        if ( !mQKD_Network.isNodeRemoved( res ) )
            break;
    }
    while (true);
    return res;
}

template < typename NetworkModel, typename IntRNG >
const typename QKD_RequestGen< NetworkModel, IntRNG >::Request 
QKD_RequestGen< NetworkModel, IntRNG >::genRequest()
{
    NodeId start = genRandomNodeId();     
    NodeId dest = genRandomNodeId();
    dclr::TimeMoment now {};
    return Request { now, now + mRequestLifetime, start, dest };   
}

template < typename NetworkModel, typename IntRNG >
void 
QKD_RequestGen<NetworkModel, IntRNG>::setRequestLifetime( dclr::Duration age )
{
    mRequestLifetime = age; 
}

template < typename NetworkModel, typename IntRNG >
void QKD_RequestGen< NetworkModel, IntRNG >::updMaxNodeId()
{
    mMaxNodeId = QKD_Node::getLastNodeId().value;
}

#endif  // QKDREQUESTGEN_HPP
