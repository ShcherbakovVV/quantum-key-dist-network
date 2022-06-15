#include "RandomQueue.hpp"


bool RandomQueue::push_request(std::shared_ptr<Request>& req) 
{ 
    _queue.push_back(req); 
    return true; 
}


std::shared_ptr<Request> RandomQueue::pop_request()
{
    if (_queue.empty())
        return {};

    _rng.reset_params(0u, _queue.size()-1);
    auto rand_elem = _rng();

    auto iter = std::find(_queue.begin(), _queue.end(), _queue.at(rand_elem));
    auto req = *iter;
    _queue.erase(iter);
    return req;
}


bool LimitedRandomQueue::push_request(std::shared_ptr<Request>& req) 
{
    return _try_push_request(_queue, req);
}


std::shared_ptr<Request> TimedRandomQueue::pop_request()
{
    auto req = RandomQueue::pop_request();
    if (req && _try_pop_request(req))
            return req;
    
    return {};
}