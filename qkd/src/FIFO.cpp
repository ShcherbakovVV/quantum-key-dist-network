#include "FIFO.hpp"


bool FIFO::push_request(std::shared_ptr<Request>& req) 
{ 
    _queue.push(req); 
    return true; 
}


std::optional<std::shared_ptr<Request>> FIFO::pop_request()
{
    if (_queue.empty())
        return {};

    auto ret = std::move(_queue.front());
    _queue.pop();
    return {ret};
}


bool LimitedFIFO::push_request(std::shared_ptr<Request>& req) 
{
    return _try_push_request(_queue, req);
}


std::optional<std::shared_ptr<Request>> TimedFIFO::pop_request()
{
    auto ret = FIFO::pop_request();
    if (ret && _try_pop_request(ret.value()))
        return ret;

    return {};
}