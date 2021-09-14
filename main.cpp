#include <iostream>
#include <chrono>

#include "src/qkdnetwork.hpp"

int main()
{
    dclr::SysClock sc {};
    QKD_Network net {sc};
    net.initNetwork();
}
