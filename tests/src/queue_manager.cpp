#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "FIFO.hpp"
#include "LIFO.hpp"
#include "PriorityQueue.hpp"
#include "QueueManager.hpp"
#include "Request.hpp"
#include "RequestQueue.hpp"
#include "RNG.hpp"

int main()
{
    int exception_count = 0;

    UniformIntRNG<unsigned> rng {1u, 10u};    

    auto n0 = PropertyTable::instance().add_entry(DescType::NODE);
    auto n1 = PropertyTable::instance().add_entry(DescType::NODE);
    PropertyTable::instance().add_property("label");
    PropertyTable::instance()(n0, "label") = "n0";
    PropertyTable::instance()(n1, "label") = "n1";

    auto fifo = std::make_shared<FIFO>();
    QueueManager qman1 {fifo};

    for (size_t i = 0; i != 15; ++i)
    {
        auto req = std::make_shared<Request>(n0, n1);
        qman1.enqueue(req);
    }
    for (size_t i = 0; i != 17; ++i)
    {
        if (!qman1.dequeue() && i == 16)
        {
            ++exception_count;  // 1
            break;
        }
    }
    
    auto lfifo = std::make_shared<LimitedLIFO>(10);
    QueueManager qman2 {lfifo};
    for (size_t i = 0; i != 11; ++i)
    {
        auto req = std::make_shared<Request>(n0, n1);
        if (!qman2.enqueue(req) && i == 10)
        {
            ++exception_count;  // 2
            break;
        }
    }
    
    for (size_t i = 0; i != 11; ++i)
    {
        if (!qman2.dequeue() && i == 10)
        {
            ++exception_count;  // 3
            break;
        }
    }
    
    auto ltprqueue = std::make_shared<LimitedTimedPriorityQueue>(10);
    QueueManager qman3 {ltprqueue};
    for (size_t i = 0; i != 11; ++i)
    {
        auto now = Clock::now();
        auto req = std::make_shared<TimedPriorityRequest>(n0, n1, now, now + 10, rng());
        auto tp_req = std::dynamic_pointer_cast<Request>(req);
        if (!qman3.enqueue(tp_req) && i == 10)
        {
            ++exception_count;  // 4
            break;
        }
    }
    
    for (size_t i = 0; i != 11; ++i)
    {
        if (!qman3.dequeue() && i == 10)
        {
            ++exception_count;  // 5
            break;
        }
    }
    
    if (exception_count == 5)
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}