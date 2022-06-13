#ifndef REQUEST_BUILDER_HPP
#define REQUEST_BUILDER_HPP

#include <memory>

#include "Global.hpp"
#include "Request.hpp"
#include "RNG.hpp"


struct RequestBuilder
{
    virtual ~RequestBuilder() {}

    virtual std::shared_ptr<Request> make_request(Descriptor, Descriptor) const;
    std::shared_ptr<Request> operator()(Descriptor, Descriptor) const;
};


class TimedRequestBuilder : public virtual RequestBuilder
{
protected: 
    Duration _req_lifetime;

public:
    TimedRequestBuilder (unsigned ms)
    :
        _req_lifetime {ms}
    {}  
    
    virtual std::shared_ptr<Request> make_request(Descriptor, Descriptor) const override;
};


class PriorityRequestBuilder : public virtual RequestBuilder
{
protected:
    mutable UniformIntRNG<unsigned> _prioritizer;
    unsigned _priority_min;
    unsigned _priority_max;
    
public:
    PriorityRequestBuilder (unsigned min, unsigned max)
    :
        _prioritizer {min, max},
        _priority_min {min},
        _priority_max {max}
    {}

    virtual std::shared_ptr<Request> make_request(Descriptor, Descriptor) const override;
};


struct TimedPriorityRequestBuilder : TimedRequestBuilder, 
                                     PriorityRequestBuilder
{
    TimedPriorityRequestBuilder (unsigned ms, unsigned min, unsigned max)
    :
        TimedRequestBuilder {ms},
        PriorityRequestBuilder {min, max}
    {}

    virtual std::shared_ptr<Request> make_request(Descriptor, Descriptor) const override;
};


#endif  // REQUEST_BUILDER_HPP