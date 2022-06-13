#include "RequestBuilder.hpp"

// PUBLIC FUNCTIONS
std::shared_ptr<Request> 
RequestBuilder::make_request(Descriptor start, Descriptor dest) const
{
    return std::make_shared<Request>(start, dest);
}


std::shared_ptr<Request> 
RequestBuilder::operator()(Descriptor start, Descriptor dest) const
{
    return make_request(start, dest);
}


std::shared_ptr<Request> 
TimedRequestBuilder::make_request(Descriptor start, Descriptor dest) const
{
    //auto now = TimePoint::clock::now();
    auto now = Clock::now();
    auto req = std::make_shared<TimedRequest>(start, dest, 
                                              now, now + _req_lifetime);
    return std::dynamic_pointer_cast<Request>(req);
}


std::shared_ptr<Request> 
PriorityRequestBuilder::make_request(Descriptor start, Descriptor dest) const
{
    auto req = std::make_shared<PriorityRequest>(start, dest, _prioritizer());
    return std::dynamic_pointer_cast<Request>(req);
}


std::shared_ptr<Request> 
TimedPriorityRequestBuilder::make_request(Descriptor start, Descriptor dest) const
{
    //auto now = TimePoint::clock::now();
    auto now = Clock::now();
    auto req = std::make_shared<TimedPriorityRequest>(start, dest, 
                                                      now, now + _req_lifetime,
                                                      _prioritizer());
    return std::dynamic_pointer_cast<Request>(req);
}
// END OF PUBLIC FUNCTIONS
