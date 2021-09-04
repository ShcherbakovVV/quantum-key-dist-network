#ifndef QKDREQUESTGEN_HPP
#define QKDREQUESTGEN_HPP

#include <chrono>

#include "common.hpp"

using std::chrono_literals::operator""ms;

template < typename Clock = common::SysClock >
struct Request
{
    using TimePoint = typename std::chrono::time_point < Clock >;

	TimePoint gen_time;  
	TimePoint exp_time;  
    Vertex& start; 
    Vertex& dest; 

    Request () = delete;

    private:
        
        //explicit
        Request ( TimePoint gen, TimePoint exp, Vertex& s, Vertex& d ); 
}

template < typename IntRNG = std::random::uniform_int_distribution >
class QKD_RequestGen
{ 
    private:
    
        QKD_Network& mQKD_Network;

        IntRNG& mIntRNG;
        Duration mRequestAge;  
        common::Id mMaxVertexId;
    
        common::Id genRandomVertexId(); 

    public:
    
        QKD_RequestGen ( QKD_Network& parent, Duration age = 60000ms ) 
                                           // время жизни заявки - 1 минута
        const Request& genRequest();
        void setRequestAge( Duration age );
        void updMaxVertexId();

    friend class Request< decltype( mQKD_Network.mClock ) >;
}

#endif  // QKDREQUESTGEN_HPP
