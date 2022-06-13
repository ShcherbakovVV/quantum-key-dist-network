#include "LIFO.hpp"
#include "RequestQueue.hpp"


bool LIFO::push_request(std::shared_ptr<Request>& req) 
{
    _queue.push(req);
    return true;
}


std::optional<std::shared_ptr<Request>> LIFO::pop_request()
{
    if (_queue.empty())
        return {};

    auto req = std::move(_queue.top());
    _queue.pop();
    return {req};
}


bool LimitedLIFO::push_request(std::shared_ptr<Request>& req) 
{
    return _try_push_request(_queue, req); 
}  


std::optional<std::shared_ptr<Request>> TimedLIFO::pop_request()
{
    auto req_opt = LIFO::pop_request();
    if (req_opt && _try_pop_request(req_opt.value()))
        return req_opt;

    return {};
}