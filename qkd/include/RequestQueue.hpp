#ifndef REQUEST_QUEUE_HPP
#define REQUEST_QUEUE_HPP

#include <memory>
#include <optional>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "Global.hpp"
#include "Request.hpp"


class RequestQueue
{
public:
    virtual ~RequestQueue () {}
    
    virtual bool push_request(std::shared_ptr<Request>&) = 0;
    virtual std::optional<std::shared_ptr<Request>> pop_request() = 0;
};


template<typename Req, typename Queue>
class LimitedQueueTrait
{
protected:
    const size_t _capacity;

    virtual bool _try_push_request(Queue&, std::shared_ptr<Req>&);

public:
    LimitedQueueTrait (size_t cap)
    :
        _capacity {cap}
    {}

    virtual ~LimitedQueueTrait () {}
};


template<typename Req>
class LimitedQueueTrait<Req, std::vector<std::shared_ptr<Req>>>
{
protected:
    const size_t _capacity;

    virtual bool _try_push_request(std::vector<std::shared_ptr<Req>>&, std::shared_ptr<Req>&);

public:
    LimitedQueueTrait (size_t cap)
    :
        _capacity {cap}
    {}

    virtual ~LimitedQueueTrait () {}
};


template<typename Req, typename Queue>
bool LimitedQueueTrait<Req, Queue>::_try_push_request(Queue& queue, std::shared_ptr<Req>& req)
{
    if (queue.size() == _capacity)
        return false;

    queue.push(req);

    return true;
}


template<typename Req>
bool LimitedQueueTrait<Req, std::vector<std::shared_ptr<Req>>>::
     _try_push_request(std::vector<std::shared_ptr<Req>>& queue, 
                       std::shared_ptr<Req>& req)
{
    if (queue.size() == _capacity)
        return false;
        
    queue.push_back(req);

    return true;
}


class TimedQueueTrait
{
protected:
    virtual bool _try_pop_request(const std::shared_ptr<Request>& req);
};

#endif  // REQUEST_QUEUE_HPP