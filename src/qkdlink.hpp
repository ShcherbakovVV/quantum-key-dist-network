#ifndef QKDLINK_HPP
#define QKDLINK_HPP

#include <memory>

#include "edge.hpp"
#include "common.hpp"

class QKD_Link;

using LinkId = typename tools::Id< QKD_Link, dclr::IdRep >;
static LinkId LINK_INVALID {-1};

class QKD_Link
{
    using id_type = LinkId;
    
    private:
        
        LinkId id;

        std::shared_ptr< Edge > mpEdge; 
        
        QKD_Link () = delete;
        QKD_Link ( std::shared_ptr< Edge >, dclr::Metrics );

    public:

        static inline LinkId last_link_id = 0;
        dclr::Metrics mMetrics;

        Edge& getEdge();

        bool operator== ( const QKD_Link& );
};

#endif  // QKDLINK_HPP
