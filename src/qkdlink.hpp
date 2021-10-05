#ifndef QKDLINK_HPP
#define QKDLINK_HPP

#include <memory>

#include "id.hpp"
#include "edge.hpp"
#include "common.hpp"
#include "qkdnode.hpp"

class QKD_Link;

using LinkId = Id< QKD_Link, dclr::IdRep >;
//static LinkId LINK_INVALID {-1};

class QKD_Link
{
    friend class QKD_Network;
    
    using id_type = LinkId;
    
    private:
        
        LinkId id;
        static inline LinkId last_link_id = 0;
        
        dclr::Metrics mMetrics;

        std::shared_ptr<Edge> mpEdge; 
        
        QKD_Link () = delete;
        QKD_Link ( std::shared_ptr<Edge>, dclr::Metrics );

    public:
        
        LinkId getLinkId();
        static LinkId getLastLinkId();
        
        dclr::Metrics getMetricsValue();
        void setMetricsValue( dclr::Metrics );

        Edge& getEdge();

        bool operator== ( const QKD_Link& );
};

#endif  // QKDLINK_HPP
