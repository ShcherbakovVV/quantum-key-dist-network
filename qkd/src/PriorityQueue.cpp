#include "PriorityQueue.hpp"


bool PriorityQueue::push_request(std::shared_ptr<Request>& req) 
{
    auto pr_req = std::dynamic_pointer_cast<PriorityRequest>(req);
    if (!pr_req)
        throw std::logic_error {"PriorityQueue::push_request(): bad request cast"};

    _queue.push(pr_req);
    return true;
}


std::shared_ptr<Request> PriorityQueue::pop_request()
{
    if (_queue.empty())
        return {};

    auto pr_req = std::move(_queue.top());
    _queue.pop();

    auto req = std::dynamic_pointer_cast<Request>(pr_req);
    if (!req)
        throw std::logic_error {"PriorityQueue::pop_request(): bad request cast"};

    return req;
}


bool LimitedPriorityQueue::push_request(std::shared_ptr<Request>& req)
{
    auto pr_req = std::dynamic_pointer_cast<PriorityRequest>(req);
    if (!pr_req)
        throw std::logic_error {"LimitedPriorityQueue::push_request(): bad request cast"};

    return _try_push_request(_queue, pr_req);
}  


std::shared_ptr<Request> TimedPriorityQueue::pop_request()
{
    auto pr_req = PriorityQueue::pop_request();
    if (!pr_req)
        return {};

    auto req = std::dynamic_pointer_cast<Request>(pr_req);
    if (!req)
        throw std::logic_error {"TimedPriorityQueue::pop_request(): bad request cast"};

    // timed request check
    if (_try_pop_request(req))
        return req;

    return {};
}