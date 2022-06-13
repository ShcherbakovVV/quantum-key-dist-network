#include "QueueManager.hpp"
#include <exception>
#include <stdexcept>


bool QueueManager::enqueue(std::shared_ptr<Request>& req)
{
    try {
        bool ret = _req_queue->push_request(req);
        BOOST_LOG_TRIVIAL(info) << "QueueManager: enqueued a request: " << *req;
        return ret;
    } catch (std::logic_error& oor) {
        BOOST_LOG_TRIVIAL(warning) << "QueueManager: enqueue dismiss";
        return false;
    }
}


std::optional<std::shared_ptr<Request>> QueueManager::dequeue()
{
    std::optional<std::shared_ptr<Request>> req_opt;
    try {
        req_opt = _req_queue->pop_request();
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(info) << exc.what();
        std::throw_with_nested(std::runtime_error {"Invalid request type usage"});
    }
    if (req_opt)
    {
        BOOST_LOG_TRIVIAL(info) << "QueueManager: dequeued a request: " << *(req_opt.value());
        return req_opt;
    }
    BOOST_LOG_TRIVIAL(warning) << "QueueManager: dequeue dismiss";
    return {};
}