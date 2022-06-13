#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <stdexcept>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "PropertyTable.hpp"


struct Request
{
    const Descriptor start_node;
    const Descriptor dest_node;
    
    Request (Descriptor start, Descriptor dest)
    :
        start_node {start},
        dest_node {dest}
    {}
    
    virtual ~Request() {}
};

std::ostream& operator<<(std::ostream&, const Request&);


struct TimedRequest : virtual Request
{
    const TimePoint gen_time;
    const TimePoint exp_time;
    
    TimedRequest (Descriptor start, Descriptor dest, TimePoint gen, TimePoint exp)
    :
        Request {start, dest},
        gen_time {gen},
        exp_time {exp}
    {
        if (gen_time > exp_time)
            throw std::logic_error {"TimedRequest: expiration and generation time mismatch"};
    }
};

std::ostream& operator<<(std::ostream&, const TimedRequest&);


struct PriorityRequest : virtual Request
{
    const unsigned priority;
    
    PriorityRequest (Descriptor start, Descriptor dest, unsigned pr)
    :
        Request {start, dest},
        priority {pr}
    {}

    virtual bool operator<(const PriorityRequest& req) const
    {
        return priority < req.priority ? true : false;
    }
};

std::ostream& operator<<(std::ostream&, const PriorityRequest&);


struct TimedPriorityRequest : TimedRequest, PriorityRequest 
{
    TimedPriorityRequest (Descriptor start, Descriptor dest, 
                          TimePoint gen, TimePoint exp,
                          unsigned pr)
    :
        Request {start, dest},
        TimedRequest {start, dest, gen, exp},
        PriorityRequest {start, dest, pr}
    {}
};

std::ostream& operator<<(std::ostream&, const TimedPriorityRequest&);

#endif  // REQUEST_HPP
