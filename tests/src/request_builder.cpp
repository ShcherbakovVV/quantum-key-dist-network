#include <cstdlib>
#include <iostream>
#include <type_traits>

#include "Descriptor.hpp"
#include "Request.hpp"
#include "RequestBuilder.hpp"

int main()
{
    int error_count = 0;

    auto n0 = DescriptorCounter::add();
    auto n1 = DescriptorCounter::add();

    RequestBuilder reqb {};
    TimedRequestBuilder treqb {100u};
    PriorityRequestBuilder preqb {1u, 10u};
    TimedPriorityRequestBuilder tpreqb {1u, 10u, 100u};

    auto r1 = reqb(n0, n1);
    auto r2 = treqb(n0, n1);
    auto r3 = preqb(n0, n1);
    auto r4 = tpreqb(n0, n1);

    static_assert(std::is_same_v<decltype(r1), std::shared_ptr<Request>>);
    static_assert(std::is_same_v<decltype(r2), std::shared_ptr<Request>>);
    static_assert(std::is_same_v<decltype(r3), std::shared_ptr<Request>>);
    static_assert(std::is_same_v<decltype(r4), std::shared_ptr<Request>>);

    auto tr = std::dynamic_pointer_cast<TimedRequest>(r2);
    if (!tr)
        ++error_count;

    auto pr = std::dynamic_pointer_cast<PriorityRequest>(r3);
    if (!pr)
        ++error_count;

    auto tpr = std::dynamic_pointer_cast<TimedPriorityRequest>(r4);
    if (!tpr)
        ++error_count;

    if (error_count == 0)
    {
        std::cout << "PASSED\n";
        return EXIT_SUCCESS;
    }
    std::cout << "FAILED\n";
    return EXIT_FAILURE;
}