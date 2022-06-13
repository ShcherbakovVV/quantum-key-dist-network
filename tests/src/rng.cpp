#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>

#include "RNG.hpp"

int main()
{
    RNG<std::uniform_int_distribution<unsigned>> rng1 {1u, 1u};
    for (unsigned j = 1; j != 10; ++j)
    {
        rng1.reset_params(j, j*j);
        for (unsigned i = 0; i != 100; ++i)
        {
            auto rand = rng1();
            if (j <= rand && rand <= j*j) ;
            else 
                throw std::logic_error {""};
        }
    }
    auto rng11 = rng1;
    auto rng111 = std::move(rng1);

    RNG<std::normal_distribution<double>> rng2 {1.f, 1.f};
    rng2();
    auto rng22 = rng2;
    auto rng222 = std::move(rng2);

    std::cout << "PASSED\n";
    return EXIT_SUCCESS;
}