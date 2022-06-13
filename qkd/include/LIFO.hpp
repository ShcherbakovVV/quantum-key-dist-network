#ifndef LIFO_HPP
#define LIFO_HPP

#include <memory>
#include <stack>
#include <stdexcept>
#include <utility>

#include "RequestQueue.hpp"


class LIFO : public RequestQueue
{
public:
    using Queue = std::stack<std::shared_ptr<Request>>;
    using Req   = Request;

protected:
    Queue _queue;

public:
    virtual bool push_request(std::shared_ptr<Request>&) override;
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;
};


class LimitedLIFO 
:
    virtual public LIFO, 
    public LimitedQueueTrait<typename LIFO::Req, typename LIFO::Queue>
{
public:
    LimitedLIFO (size_t cap)
    :
        LimitedQueueTrait {cap}
    {}

    virtual bool push_request(std::shared_ptr<Request>&) override;                                         
};


class TimedLIFO 
:
    virtual public LIFO, 
    public TimedQueueTrait
{
public:
    virtual std::optional<std::shared_ptr<Request>> pop_request() override;                                            
};


class LimitedTimedLIFO 
:
    public LimitedLIFO,
    public TimedLIFO
{
public:
    LimitedTimedLIFO (size_t cap)
    :
        LimitedLIFO {cap}
    {}                                      
};

#endif  // LIFO_HPP