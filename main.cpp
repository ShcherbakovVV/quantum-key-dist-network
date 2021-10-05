#include <iostream>
#include <chrono>

#include "src/qkdnetwork.hpp"

int main()
{
    QKD_Network net {};
    NodeId n1 = net.addNode();
    NodeId n2 = net.addNode();
    LinkId l = net.addLink( net.getNodeById(n1), net.getNodeById(n2) );
}
