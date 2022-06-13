#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>

#include <boost/log/trivial.hpp>

#include "Global.hpp"
#include "Module.hpp"
#include "Network.hpp"
#include "RequestQueue.hpp"


/* TODO:
 *   - dismiss of request
 */

class QueueManager : public Module
{
protected:
    const std::shared_ptr<RequestQueue> _req_queue;
    
    bool _serviced = true;
    TimePoint _at;

public:
    QueueManager (const std::shared_ptr<RequestQueue> queue)
    :
        Module {ModuleType::QUEUE_MANAGER},
        _req_queue {queue}
    {}

    template<typename ServiceDistrib>
    bool service(const ServiceDistrib&);

    virtual QueueManager&       access()       override { return *this; }
    virtual const QueueManager& access() const override { return *this; }

    virtual bool enqueue(std::shared_ptr<Request>& req);
    virtual std::optional<std::shared_ptr<Request>> dequeue();
};


template<typename ServiceDistrib>
bool QueueManager::service(const ServiceDistrib& serv)
{
    if (_serviced)
    {
        Duration serv_time;
        do {
            serv_time = static_cast<Duration>(serv());
        } while (serv_time < network().step());
        
        _at = network().curr() + serv_time;
        _serviced = false;
    }
    if (_at <= network().curr())
        _serviced = true;
        
    return _serviced;
}

#endif  // QUEUE_MANAGER_HPP
