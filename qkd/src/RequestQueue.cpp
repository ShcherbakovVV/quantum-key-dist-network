#include "RequestQueue.hpp"


bool TimedQueueTrait::_try_pop_request(const std::shared_ptr<Request>& req)
{
    const auto& timed_req = std::dynamic_pointer_cast<TimedRequest>(req);
    if (!timed_req)
        throw std::logic_error {"TimedQueueTrait: bad request cast"};

    if (timed_req->exp_time <= Clock::now())  // if outdated
        return false;
    
    return true;
}