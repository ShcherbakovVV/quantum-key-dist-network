#ifndef QKDNODE_HPP
#define QKDNODE_HPP

#include <memory>
#include <string>

#include "id.hpp"
#include "common.hpp"
#include "vertex.hpp" 

class QKD_Node;

using NodeId = Id< QKD_Node, dclr::IdRep >;

class QKD_Node
{
    friend class QKD_Network;

    using id_type = NodeId;
    
    private:
        
        NodeId id;
        static inline NodeId last_node_id = 0;

        std::shared_ptr< Vertex > mpVertex; 

        QKD_Node () = delete; 
        QKD_Node ( std::shared_ptr<Vertex>, const std::string& );

    public:
        
        std::string label;

        NodeId getNodeId();
        static NodeId getLastNodeId();

        Vertex& getVertex();

        bool operator== ( const QKD_Node& );
};

#endif  // QKDNODE_HPP
