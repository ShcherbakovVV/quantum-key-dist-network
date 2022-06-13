#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "AdjacencyMatrix.hpp"

//    | 0  1  2  3  4  5
// ---|------------------
//  0 | 0  .  .  . . . .
//  1 | 1  2  .  . . . .
//  2 | 3  4  5  . . . .
//  3 | 6  7  8  9 . . .
//  4 | 10 11 12 13 14 .
//  5 | 15 16 17 18 19 20
int main() 
{
    AdjacencyMatrix am {};
    
        DescriptorCounter::add();
    auto n0 = am.add_node();
    assert(am.is_node(n0));
    assert(!am.is_link(n0));
    
        DescriptorCounter::add();
    auto n1 = am.add_node();
    assert(am.is_node(n1));
    assert(!am.is_link(n1));
    
        DescriptorCounter::add();
    auto l2 = am.add_link(n0, n1);
    assert(!am.is_node(l2));
    assert(am.is_link(l2));
    
        DescriptorCounter::add();
    auto n3 = am.add_node();
    assert(am.is_node(n3));
    assert(!am.is_link(n3));
    
        DescriptorCounter::add();
    auto n4 = am.add_node();
    assert(am.is_node(n4));
    assert(!am.is_link(n4));
    
        DescriptorCounter::add();
    auto l5 = am.add_link(n1, n3);
    assert(!am.is_node(l5));
    assert(am.is_link(l5));
    
        DescriptorCounter::add();
    auto l6 = am.add_link(n1, n4);
    assert(!am.is_node(l6));
    assert(am.is_link(l6));
    
        DescriptorCounter::add();
    auto n7 = am.add_node();
    assert(am.is_node(n7));
    assert(!am.is_link(n7));
    
        DescriptorCounter::add();
    auto l8 = am.add_link(n4, n7);
    assert(!am.is_node(l8));
    assert(am.is_link(l8));
    
    auto adj_n1 = am[n1];
    std::sort(adj_n1.begin(), adj_n1.end());
    std::vector<Descriptor> adj_n1_check = {l2, l5, l6};
    assert((adj_n1.size() == adj_n1_check.size()));
    for (size_t i = 0; i != adj_n1_check.size(); ++i)
        assert((adj_n1[i] == adj_n1_check[i]));
    
    auto adj_l8 = am[l8];
    std::sort(adj_l8.begin(), adj_l8.end());
    std::vector<Descriptor> adj_l8_check = {n4, n7};
    assert((adj_l8.size() == adj_l8_check.size()));
    for (size_t i = 0; i != adj_l8_check.size(); ++i)
        assert((adj_l8[i] == adj_l8_check[i]));
        
    auto between_n0_n1 = am(n0, n1);
    assert((between_n0_n1 == l2));
    
    auto between_l6_l8 = am(l6, l8);
    assert((between_l6_l8 == n4));
    
    int exceptions_caught = 0;
    try {
        am[9];
    } catch (std::exception& exc) {
        ++exceptions_caught;  // 1
    }
    
    try {
        am[-1];
    } catch (std::exception& exc) {
        ++exceptions_caught;  // 2
    }
    
    try {
        am(0, 9);
    } catch (std::exception& exc) {
        ++exceptions_caught; // 3
    }
    
    try {
        am(0, -1);
    } catch (std::exception& exc) {
        ++exceptions_caught;  // 4
    }
    
    if (exceptions_caught == 4)
    {
        std::cout << "PASSED\n";
        return EXIT_SUCCESS;
    }
    std::cout << "FAILED\n";
    
    return EXIT_FAILURE;
}
