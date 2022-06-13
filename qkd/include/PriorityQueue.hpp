#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>

#include "RequestQueue.hpp"


class PriorityQueue : public RequestQueue
{
public:
    using Queue = std::priority_queue<std::shared_ptr<PriorityRequest>>;
    using Req   = PriorityRequest;

protected:
    Queue _queue;
    
public:
    virtual bool push_request(std::shared_ptr<Request>&) override;
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;
};


class LimitedPriorityQueue
: 
    virtual public PriorityQueue,
    public LimitedQueueTrait<typename PriorityQueue::Req, typename PriorityQueue::Queue>
{
public:
    LimitedPriorityQueue (size_t cap)
    :
        LimitedQueueTrait {cap}
    {}

    virtual bool push_request(std::shared_ptr<Request>&) override;                       
};


class TimedPriorityQueue
: 
    virtual public PriorityQueue,
    public TimedQueueTrait
{
public:
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;
};


class LimitedTimedPriorityQueue
: 
    public LimitedPriorityQueue,
    public TimedPriorityQueue
{
public:
    LimitedTimedPriorityQueue (size_t cap)
    :
        LimitedPriorityQueue {cap}
    {}
};

#endif  // PRIORITY_QUEUE_HPP