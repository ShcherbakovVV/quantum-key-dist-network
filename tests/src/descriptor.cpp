#include <cassert>
#include <cstdlib>
#include <iostream>

#include "Descriptor.hpp"

int main() 
{
    Descriptor d0 = DescriptorCounter::add();
    Descriptor d1 = DescriptorCounter::add();
    Descriptor d2 = DescriptorCounter::get();
    assert((d0 == 0));
    assert((d1 == 1));
    assert((d2 == 1));
    
    try {
        while (true)
            DescriptorCounter::add();
    } catch (std::exception& exc) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
} 

