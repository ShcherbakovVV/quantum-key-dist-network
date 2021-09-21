#ifndef QKDNODE_HPP
#define QKDNODE_HPP

#include <memory>
#include <string>

#include "common.hpp"
#include "vertex.hpp" 

class QKD_Node;

using NodeId = typename tools::Id< QKD_Node, dclr::IdRep >;
static NodeId NODE_INVALID {-1};

class QKD_Node
{
    using id_type = NodeId;
    
    private:
        
        NodeId id;

        std::shared_ptr< Vertex > mpVertex; 

        QKD_Node () = delete; 
        QKD_Node ( std::shared_ptr< Vertex >, std::string );

    public:

        static inline NodeId last_node_id = 0;
        std::string label;

        Vertex& getVertex();

        bool operator== ( const QKD_Node& );
};

#endif  // QKDNODE_HPP
