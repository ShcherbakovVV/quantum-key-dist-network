#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <utility>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "Network.hpp"
#include "PriorityQueue.hpp"
#include "RNG.hpp"


class NetworkTest : public Network
{
public:
    void tick(Duration dur)
    {
        clock_tick(dur);
    }
};


int main()
{
    int exception_count = 0;

    UniformIntRNG<unsigned> rng {0u, 15u};

    auto n0 = DescriptorCounter::add();
    auto n1 = DescriptorCounter::add();

    PriorityQueue prqueue {};
    LimitedPriorityQueue lprqueue {10};
    TimedPriorityQueue tprqueue {};
    LimitedTimedPriorityQueue ltprqueue {10};

    // PriorityQueue, pushing
    for (size_t i = 0; i != 15; ++i)
    {
        auto req = std::make_shared<PriorityRequest>(n0, n1,
                                                     rng());
        auto preq = std::dynamic_pointer_cast<Request>(req);
        prqueue.push_request(preq);
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto req = std::make_shared<PriorityRequest>(n0, n1,
                                                     rng());
        auto preq = std::dynamic_pointer_cast<Request>(req); 
        if (!lprqueue.push_request(preq) && i == 10)
        {
            ++exception_count;  // 1
            break;
        }
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto tp = Clock::now();
        auto req = std::make_shared<TimedPriorityRequest>(n0, n1,
                                                          tp, tp + 10,
                                                          rng());
        auto tpreq = std::dynamic_pointer_cast<Request>(req); 
        tprqueue.push_request(tpreq);
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto tp = Clock::now();
        auto req = std::make_shared<TimedPriorityRequest>(n0, n1,
                                                          tp, tp + 10,
                                                          rng());
        auto tpreq = std::dynamic_pointer_cast<Request>(req); 
 
        if (!ltprqueue.push_request(tpreq) && i == 10)
        {
            ++exception_count;  // 2
            break;
        }
    }
    // end of PriorityQueue pushing

    // PriorityQueue, popping
    for (size_t i = 0; i != 16; ++i)
    {
        if (!prqueue.pop_request() && i == 15)
        {
            ++exception_count;  // 3
            break;
        }
    }

    auto req2 = std::make_shared<Request>(n0, n1);
    try {
        prqueue.push_request(req2);
        prqueue.pop_request();
    } catch (...) {
        ++exception_count;  // 4
    }

    for (size_t i = 0; i != 16; ++i)
    {
        if (!lprqueue.pop_request() && i == 10)
        {
            ++exception_count;  // 5
            break;
        }
    }

    try {
        lprqueue.push_request(req2);
        lprqueue.pop_request();
    } catch (...) {
        ++exception_count;  // 6
    }

    for (size_t i = 0; i != 16; ++i)
    {
        if (!tprqueue.pop_request() && i == 15)
        {
            ++exception_count;  // 7
            break;
        }
    }

    try {
        tprqueue.push_request(req2);
        tprqueue.pop_request();
    } catch (...) {
        ++exception_count;  // 8
    }

    auto req3 = std::make_shared<PriorityRequest>(n0, n1, rng());
    auto t_req3 = std::dynamic_pointer_cast<Request>(req3);
    try {
        tprqueue.push_request(t_req3);
        tprqueue.pop_request();
    } catch (...) {
        ++exception_count;  // 9
    }

    for (size_t i = 0; i != 16; ++i)
    {
        if (!ltprqueue.pop_request() && i == 10)
        {
            ++exception_count;  // 10
            break;
        }
    }

    try {
        ltprqueue.push_request(req2);
        ltprqueue.pop_request();
    } catch (...) {
        ++exception_count;  // 11
    }

    try {
        ltprqueue.push_request(t_req3);
        ltprqueue.pop_request();
    } catch (...) {
        ++exception_count;  // 12
    }
    // end of PriorityQueue popping

    NetworkTest net {};

    auto now = Clock::now();
    auto treq1 = std::make_shared<TimedPriorityRequest>(n0, n1, now, now + 10, rng());
    auto treq2 = std::make_shared<TimedPriorityRequest>(n0, n1, now, now + 10, rng());
    auto t_req1 = std::dynamic_pointer_cast<Request>(treq1); 
    auto t_req2 = std::dynamic_pointer_cast<Request>(treq2); 

    TimedPriorityQueue tprq {};
    tprq.push_request(t_req1);

    LimitedTimedPriorityQueue ltprq {10};
    ltprq.push_request(t_req2);

    net.tick(200);
    if (!tprq.pop_request())
        ++exception_count;  // 13

    if (!ltprq.pop_request())
        ++exception_count;  // 14

    if (exception_count == 14)
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}