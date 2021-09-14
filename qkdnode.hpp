#ifndef QKDNODE_HPP
#define QKDNODE_HPP

#include <memory>
#include <string>

#include "common.hpp"
#include "vertex.hpp" 

class QKD_Node;

using NodeId = tools::Id< QKD_Node, dclr::IdRep >;
NodeId NODE_INVALID {-1};

class QKD_Node
{
    private:
        
        NodeId id;

        //std::shared_ptr< Vertex > mpVertex; 
        Vertex& mVertex;

        QKD_Node () = delete; 
        QKD_Node ( Vertex&, std::string )

    public:

        static inline dclr::Id last_node_id = 0;
        std::string label;

        Vertex& getVertex();

        bool operator== ( const QKD_Node& );
};

#endif  // QKDNODE_HPP
