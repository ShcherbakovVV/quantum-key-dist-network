#ifndef RANDOM_QUEUE_HPP
#define RANDOM_QUEUE_HPP

#include <cstddef>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>
#include <utility>

#include "RequestQueue.hpp"
#include "RNG.hpp"


class RandomQueue : public RequestQueue
{
public:
    using Queue = std::vector<std::shared_ptr<Request>>;
    using Req   = Request;

protected:
    Queue _queue;
    UniformIntRNG<size_t> _rng {0u, 0u};
    
public:   
    virtual bool push_request(std::shared_ptr<Request>&) override;
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;
};


class LimitedRandomQueue
:
    virtual public RandomQueue, 
    public LimitedQueueTrait<typename RandomQueue::Req, typename RandomQueue::Queue>
{
public:
    LimitedRandomQueue (size_t cap)
    :
        LimitedQueueTrait {cap}
    {}

    virtual bool push_request(std::shared_ptr<Request>& req) override;
};


class TimedRandomQueue 
: 
    virtual public RandomQueue, 
    public TimedQueueTrait
{
public:
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;
};


class LimitedTimedRandomQueue 
: 
    public LimitedRandomQueue, 
    public TimedRandomQueue 
{
public:
    LimitedTimedRandomQueue (size_t cap)
    :
        LimitedRandomQueue {cap}
    {}
};

#endif  // RANDOM_QUEUE_HPP