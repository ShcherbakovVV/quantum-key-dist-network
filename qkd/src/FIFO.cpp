#include "FIFO.hpp"


bool FIFO::push_request(std::shared_ptr<Request>& req) 
{ 
    _queue.push(req); 
    return true; 
}


std::shared_ptr<Request> FIFO::pop_request()
{
    if (_queue.empty())
        return {};

    auto req = _queue.front();
    _queue.pop();
    return req;
}


bool LimitedFIFO::push_request(std::shared_ptr<Request>& req) 
{
    return _try_push_request(_queue, req);
}


std::shared_ptr<Request> TimedFIFO::pop_request()
{
    auto req = FIFO::pop_request();
    if (req && _try_pop_request(req))
        return req;
    
    return {};
}