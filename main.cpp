#include <iostream>
#include "qkdtopology.hpp"

int main()
{
    QKD_Topology t{};
    //std::pair<Edge&, Vertex&> pr 
    auto pr {t.createEdge(t.root)};
    
}
