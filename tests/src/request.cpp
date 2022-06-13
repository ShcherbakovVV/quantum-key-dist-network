#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "Network.hpp"
#include "Request.hpp"


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

    auto n0 = DescriptorCounter::add();
    auto n1 = DescriptorCounter::add();

    NetworkTest net {};
    net.tick(100);

    auto req = std::make_shared<Request>(n0, n1);
    auto treq = std::make_shared<TimedRequest>(n0, n1, 
                                               Clock::now(),
                                               Clock::now() + 10);
    try {
        auto treq2 = std::make_shared<TimedRequest>(n0, n1, 
                                                    Clock::now(),
                                                    Clock::now() - 10);
    } catch (...) {
        ++exception_count;  // 1
    }
    auto preq = std::make_shared<PriorityRequest>(n0, n1, 10);
    auto tpreq = std::make_shared<TimedPriorityRequest>(n0, n1, 
                                                        Clock::now(),
                                                        Clock::now() + 10,
                                                        10);
    try {
        auto tpreq2 = std::make_shared<TimedRequest>(n0, n1,
                                                     Clock::now(),
                                                     Clock::now() - 10);
    } catch (...) {
        ++exception_count;  // 2
    }

    if (exception_count == 2)
    {
        std::cout << "PASSED\n";
        return EXIT_SUCCESS;
    }
    std::cout << "FAILED\n";
    return EXIT_FAILURE;
}