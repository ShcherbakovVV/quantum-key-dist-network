#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "Descriptor.hpp"
#include "Path.hpp"

int main()
{
    Descriptor start_node = DescriptorCounter::add();  //0
    Descriptor dest_node = DescriptorCounter::add();  //1
    Descriptor node_not_in_path = DescriptorCounter::add();  //2
    Path path1 {start_node, dest_node};

    assert((path1.start() == start_node && path1.dest() == dest_node));
    assert((path1.in_path(start_node) && path1.in_path(dest_node)));
    assert((!path1.in_path(node_not_in_path)));
    assert((!path1));

    Descriptor link1 = DescriptorCounter::add();  //3
    path1.append_link_back(link1);
    assert((path1));

    Descriptor node1 = DescriptorCounter::add();  //4
    path1.append_node_back(node1);
    assert((!path1));

    Descriptor link2 = DescriptorCounter::add();  //5
    path1.append_link_back(link2);
    assert((path1));

    Descriptor node2 = DescriptorCounter::add();  //6
    path1.append_node_front(node2);
    assert((!path1));

    Descriptor link3 = DescriptorCounter::add();  //7
    path1.append_link_front(link3);
    assert((path1));

    assert((path1.start() == start_node && path1.dest() == dest_node));
    assert((path1.in_path(link1) 
            && path1.in_path(node1)
            && path1.in_path(link2)
            && path1.in_path(node2)
            && path1.in_path(link3)));

    assert((path1.path_data() == std::vector<Descriptor> 
                                    {start_node, link3, node2, link1, node1, link2, dest_node}));

    assert((path1));    
    return EXIT_SUCCESS;
}