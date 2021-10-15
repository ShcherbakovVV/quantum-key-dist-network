#include <iostream>
#include <chrono>

#include "src/qkdnetwork.hpp"

int main()
{
    QKD_Network net {};
    NodeId n1 = net.addNode();
    NodeId n2 = net.addNode();
    NodeId n3 = net.addNode();
    NodeId n4 = net.addNode();
    NodeId n5 = net.addNode();
    NodeId n6 = net.addNode();
    NodeId n7 = net.addNode();
    NodeId n8 = net.addNode();
    NodeId n9 = net.addNode();
    LinkId l1 = net.addLink( n1, n2 );
    LinkId l2 = net.addLink( n2, n3 );
    LinkId l3 = net.addLink( n3, n4 );
    LinkId l4 = net.addLink( n3, n5 );
    LinkId l5 = net.addLink( n3, n1 );
    LinkId l6 = net.addLink( n4, n5 );
    LinkId l7 = net.addLink( n5, n6 );
    LinkId l8 = net.addLink( n5, n2 );
    net.simulate();
}
