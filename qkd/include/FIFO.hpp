#ifndef FIFO_HPP
#define FIFO_HPP

#include <memory>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>

#include "RequestQueue.hpp"


class FIFO : public RequestQueue
{
public:
    using Queue = std::queue<std::shared_ptr<Request>>;
    using Req   = Request;

protected:
    Queue _queue;
    
public:   
    virtual bool push_request(std::shared_ptr<Request>&) override;
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;
};


class LimitedFIFO
:
    virtual public FIFO, 
    public LimitedQueueTrait<typename FIFO::Req, typename FIFO::Queue>
{
public:
    LimitedFIFO (size_t cap)
    :
        LimitedQueueTrait {cap}
    {}

    virtual bool push_request(std::shared_ptr<Request>& req) override;
};


class TimedFIFO 
: 
    virtual public FIFO, 
    public TimedQueueTrait
{
public:
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;
};


class LimitedTimedFIFO 
: 
    public LimitedFIFO, 
    public TimedFIFO
{
public:
    LimitedTimedFIFO (size_t cap)
    :
        LimitedFIFO {cap}
    {}
};

#endif  // FIFO_HPP