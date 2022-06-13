#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <utility>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "FIFO.hpp"
#include "RNG.hpp"

using namespace std::literals::chrono_literals;

int main()
{
    int exception_count = 0;

    UniformIntRNG<unsigned> rng {0u, 15u};

    auto n0 = DescriptorCounter::add();
    auto n1 = DescriptorCounter::add();

    FIFO fifo {};
    LimitedFIFO lfifo {10};
    TimedFIFO tfifo {};
    LimitedTimedFIFO ltfifo {10};

    // FIFO, pushing
    for (size_t i = 0; i != 15; ++i)
    {
        auto req = std::make_shared<Request>(n0, n1);
        fifo.push_request(req);
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto req = std::make_shared<Request>(n0, n1);
        try {
            lfifo.push_request(req);
        } catch (...) {
            if (i == 10)
            {
                ++exception_count;  // 1
                break;
            }
        }
    }
    
    for (size_t i = 0; i != 15; ++i)
    {
        auto tp = Clock::now(); // TimePoint::clock::now();
        auto req = std::make_shared<TimedRequest>(n0, n1,
                                                  tp,
                                                  tp + 10); // tp + 10ms);
        auto treq = std::dynamic_pointer_cast<Request>(req);
        tfifo.push_request(treq);
    }
    for (size_t i = 0; i != 15; ++i)
    {
        auto tp = Clock::now(); // TimePoint::clock::now();
        auto req = std::make_shared<TimedRequest>(n0, n1,
                                                  tp,
                                                  tp + 10); // tp + 10ms);
        auto treq = std::dynamic_pointer_cast<Request>(req);
        try { 
            ltfifo.push_request(req);
        } catch (...) {
            if (i == 10)
            {
                ++exception_count;  // 2
                break;
            }
        }
    }
    
    // end of FIFO pushing

    // FIFO, popping
    for (size_t i = 0; i != 16; ++i)
    {
        try {
            auto req = fifo.pop_request();
        } catch (...) {
            if (i == 15)
            {
                ++exception_count;  // 3
                break;
            }
        }
    }
    
    for (size_t i = 0; i != 16; ++i)
    {
        try {
            auto req = lfifo.pop_request();
        } catch (...) {
            if (i == 10)
            {
                ++exception_count;  // 4
                break;
            }
        }
    }
    
    for (size_t i = 0; i != 16; ++i)
    {
        try {
            auto req = tfifo.pop_request();
        } catch (...) {
            if (i == 15)
            {
                ++exception_count;  // 5
                break;
            }
        }
    }
    

    auto req2 = std::make_shared<Request>(n0, n1);
    try {
        tfifo.push_request(req2);
        tfifo.pop_request();
    } catch (...) {
        ++exception_count;  // 6
    }

    for (size_t i = 0; i != 16; ++i)
    {
        try {
            auto req = ltfifo.pop_request();
        } catch (...) {
            if (i == 10)
            {
                ++exception_count;  // 7
                break;
            }
        }
    }

    try {
        ltfifo.push_request(req2);
        ltfifo.pop_request();
    } catch (...) {
        ++exception_count;  // 8
    }
    // end of FIFO popping


    auto now = Clock::now(); // TimePoint::clock::now();
    auto treq1 = std::make_shared<TimedRequest>(n0, n1, now, now + 10); // now + 10ms);
    auto treq2 = std::make_shared<TimedRequest>(n0, n1, now, now + 10); // now + 10ms);
    auto t_req1 = std::dynamic_pointer_cast<Request>(treq1);
    auto t_req2 = std::dynamic_pointer_cast<Request>(treq2);

    TimedFIFO tfifo2 {};
    tfifo2.push_request(t_req2);

    LimitedTimedFIFO ltfifo2 {10};
    ltfifo2.push_request(t_req2);

    std::this_thread::sleep_for(200ms);
    try {
        tfifo2.pop_request();
    } catch (...) {
        ++exception_count;  // 9
    }
    try {
        ltfifo2.pop_request();
    } catch (...) {
        ++exception_count;  // 10
    }

    if (exception_count == 10)
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}