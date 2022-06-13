#include "RandomQueue.hpp"


bool RandomQueue::push_request(std::shared_ptr<Request>& req) 
{ 
    _queue.push_back(req); 
    return true; 
}


std::optional<std::shared_ptr<Request>> RandomQueue::pop_request()
{
    if (_queue.empty())
        return {};

    _rng.reset_params(0u, _queue.size()-1);
    auto rand_elem = _rng();

    auto iter = std::find(_queue.begin(), _queue.end(), _queue.at(rand_elem));
    auto ret = std::move(*iter);
    _queue.erase(iter);
    return {ret};
}


bool LimitedRandomQueue::push_request(std::shared_ptr<Request>& req) 
{
    return _try_push_request(_queue, req);
}


std::optional<std::shared_ptr<Request>> TimedRandomQueue::pop_request()
{
    auto ret = RandomQueue::pop_request();
    if (ret && _try_pop_request(ret.value()))
        return ret;

    return {};
}