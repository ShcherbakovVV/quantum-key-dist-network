#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <utility>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "LIFO.hpp"
#include "Network.hpp"
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

    LIFO lifo {};
    LimitedLIFO llifo {10};
    TimedLIFO tlifo {};
    LimitedTimedLIFO ltlifo {10};

    // LIFO, pushing
    for (size_t i = 0; i != 15; ++i)
    {
        auto req = std::make_shared<Request>(n0, n1);
        lifo.push_request(req);
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto req = std::make_shared<Request>(n0, n1);
        if (!llifo.push_request(req) && i == 10)
        {
            ++exception_count;  // 1
            break;
        }
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto tp = Clock::now();
        auto req = std::make_shared<TimedRequest>(n0, n1,
                                                  tp,
                                                  tp + 10);
        auto treq = std::dynamic_pointer_cast<Request>(req); 
        tlifo.push_request(treq);
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto tp = Clock::now();
        auto req = std::make_shared<TimedRequest>(n0, n1,
                                                  tp,
                                                  tp + 10);
        auto treq = std::dynamic_pointer_cast<Request>(req); 
        if (!ltlifo.push_request(treq) && i == 10)
        {
            ++exception_count;  // 2
            break;
        }
    }
    // end of LIFO pushing

    // LIFO, popping
    for (size_t i = 0; i != 16; ++i)
    {
        if (!lifo.pop_request() && i == 15)
        {
            ++exception_count;  // 3
            break;
        }
    }
    for (size_t i = 0; i != 16; ++i)
    {
        if (!llifo.pop_request() && i == 10)
        {
            ++exception_count;  // 4
            break;
        }
    }
    for (size_t i = 0; i != 16; ++i)
    {
        if (!tlifo.pop_request() && i == 15)
        {
            ++exception_count;  // 5
            break;
        }
    }

    auto req2 = std::make_shared<Request>(n0, n1);
    try {
        tlifo.push_request(req2);
        tlifo.pop_request();
    } catch (...) {
        ++exception_count;  // 6
    }

    for (size_t i = 0; i != 16; ++i)
    {
        if (i == 10 && ltlifo.pop_request())
        {
            ++exception_count;  // 7
            break;
        }
    }

    try {
        ltlifo.push_request(req2);
        ltlifo.pop_request();
    } catch (...) {
        ++exception_count;  // 8
    }
    // end of LIFO popping

    NetworkTest net {};

    auto now = Clock::now();
    auto treq1 = std::make_shared<TimedRequest>(n0, n1, now, now + 10);
    auto treq2 = std::make_shared<TimedRequest>(n0, n1, now, now + 10);
    auto t_req1 = std::dynamic_pointer_cast<Request>(treq1); 
    auto t_req2 = std::dynamic_pointer_cast<Request>(treq2); 

    TimedLIFO tlifo2 {};
    tlifo2.push_request(t_req1);

    LimitedTimedLIFO ltlifo2 {10};
    ltlifo2.push_request(t_req2);

    net.tick(200);
    if (!tlifo2.pop_request())
        ++exception_count;  // 9

    if (!ltlifo2.pop_request())
        ++exception_count;  // 10

    if (exception_count == 10)
        return EXIT_SUCCESS;
    
    return EXIT_FAILURE;
}