#include "LIFO.hpp"


bool LIFO::push_request(std::shared_ptr<Request>& req) 
{
    _queue.push(req);
    return true;
}


std::shared_ptr<Request> LIFO::pop_request()
{
    if (_queue.empty())
        return {};

    auto req = _queue.top();
    _queue.pop();
    return req;
}


bool LimitedLIFO::push_request(std::shared_ptr<Request>& req) 
{
    return _try_push_request(_queue, req); 
}  


std::shared_ptr<Request> TimedLIFO::pop_request()
{
    auto req = LIFO::pop_request();
    if (req && _try_pop_request(req))
        return req;

    return {};
}