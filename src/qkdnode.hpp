#ifndef QKDNODE_HPP
#define QKDNODE_HPP

#include <memory>
#include <string>

#include <boost/log/trivial.hpp>

#include "id.hpp"
#include "common.hpp"
#include "vertex.hpp" 

class QKD_Node;

using NodeId = Id<QKD_Node, dclr::IdRep>;

class QKD_Node
{
    friend class QKD_Network;
    friend std::ostream& operator<< ( std::ostream&, const QKD_Node& );

    using id_type = NodeId;
    
    private:
        
        NodeId id;
        static inline NodeId last_node_id = 0;

        std::shared_ptr<Vertex> mpVertex; 

        QKD_Node () = delete; 
        // а если строка - временный объект?
        QKD_Node ( const std::shared_ptr<Vertex>&, const std::string& );

    public:
        
        ~QKD_Node ();
        
        std::string label;

        NodeId getNodeId() const;
        static NodeId getLastNodeId();

        Vertex& getVertex() const;
        VertexId getVertexId() const;

        bool operator== ( const QKD_Node& ) const;
};

std::ostream& operator<< ( std::ostream&, const QKD_Node& );

#endif  // QKDNODE_HPP
