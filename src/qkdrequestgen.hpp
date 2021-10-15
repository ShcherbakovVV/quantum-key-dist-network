#ifndef QKDREQUESTGEN_HPP
#define QKDREQUESTGEN_HPP

#include <chrono>
#include <random>

#include <boost/log/trivial.hpp>

#include "edge.hpp"
#include "common.hpp"
#include "vertex.hpp"
#include "uintrng.hpp"
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
RequestType<TPoint>::RequestType (TPoint gen, TPoint exp, NodeId s, NodeId d) 
:
    gen_time { gen }, 
    exp_time { exp },  
    start { s },
    dest { d }
{}

using Request = RequestType<dclr::TimeMoment>;

template <typename NetworkModel,
          typename IntRNG = UIntRandNumGen<dclr::IdRep,
                                           std::uniform_int_distribution>>
class QKD_RequestGen
{ 
    private:
    
        NetworkModel& mQKD_Network;

        IntRNG mIntRNG;
        dclr::Duration mRequestLifetime;
    
        NodeId genRandomNodeId();  // не const из-за IntRNG

    public:
    
        QKD_RequestGen () = delete;
        QKD_RequestGen ( NetworkModel&, dclr::Duration age = 60000ms );
        
        ~QKD_RequestGen ();
                                           
        Request genRequest();  // не const из-за IntRNG
        void setRequestLifetime( dclr::Duration );
        void updGenParams();
};

template <typename NetworkModel, typename IntRNG>
QKD_RequestGen<NetworkModel, IntRNG>::QKD_RequestGen
( NetworkModel& parent, dclr::Duration age ) 
:
    mQKD_Network     { parent },
    mRequestLifetime { age },
    mIntRNG          { 0, 
                       QKD_Node::getLastNodeId().value,
                       std::uniform_int_distribution<dclr::IdRep> 
                           { 0, dclr::ID_MAX } }
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_RequestGen";
}

template <typename NetworkModel, typename IntRNG>
QKD_RequestGen<NetworkModel, IntRNG>::~QKD_RequestGen ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_RequestGen";
}
 
template <typename NetworkModel, typename IntRNG>
NodeId QKD_RequestGen<NetworkModel, IntRNG>::genRandomNodeId()
{
    NodeId res;
    do {
        res = mIntRNG();
        if ( !mQKD_Network.isNodeRemoved( res ) )
            break;
    } 
    while (true);
    //BOOST_LOG_TRIVIAL(trace) << "QKD_RequestGen: selected " 
        //<< mQKD_Network.getNodeById( res );
    return res;
}

template <typename NetworkModel, typename IntRNG>
Request QKD_RequestGen<NetworkModel, IntRNG>::genRequest()
{
    NodeId start, dest; 
    start = genRandomNodeId();
    do
        dest = genRandomNodeId();
    while ( start == dest ); 

    dclr::TimeMoment now {};
    dclr::TimeMoment expir = now + mRequestLifetime;
    BOOST_LOG_TRIVIAL(trace) 
        << "QKD_RequestGen: generated Request {" 
        << start << ", " << dest << "} at " 
        << now << ", expires at " << expir;
    return Request { now, expir, start, dest };   
}

template <typename NetworkModel, typename IntRNG>
void 
QKD_RequestGen<NetworkModel, IntRNG>::setRequestLifetime(dclr::Duration age)
{
    mRequestLifetime = age;
    BOOST_LOG_TRIVIAL(trace)
        << "QKD_RequestGen: set Request Lifetime to " << age;
}

template <typename NetworkModel, typename IntRNG>
void QKD_RequestGen<NetworkModel, IntRNG>::updGenParams()
{
    mIntRNG.max_out( QKD_Node::getLastNodeId().value );
    BOOST_LOG_TRIVIAL(trace) << "QKD_RequestGen: RNG params has been updated";
}

#endif  // QKDREQUESTGEN_HPP
