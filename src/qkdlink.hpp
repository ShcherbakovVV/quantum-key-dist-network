#ifndef QKDLINK_HPP
#define QKDLINK_HPP

#include <memory>

#include <boost/log/trivial.hpp>

#include "id.hpp"
#include "edge.hpp"
#include "common.hpp"
#include "qkdnode.hpp"

class QKD_Link;

using LinkId = Id<QKD_Link, dclr::IdRep>;

class QKD_Link
{
    friend class QKD_Network;
    friend std::ostream& operator<< ( std::ostream&, const QKD_Link& );
    
    using id_type = LinkId;
    
    private:
        
        LinkId id;
        static inline LinkId last_link_id = 0;
        
        dclr::Metrics mMetrics;

        std::shared_ptr<Edge> mpEdge; 
        
        QKD_Link () = delete;
        QKD_Link ( const std::shared_ptr<Edge>&, dclr::Metrics );

    public:
        
        ~QKD_Link ();
        
        LinkId getLinkId() const;
        static LinkId getLastLinkId();
        
        dclr::Metrics getMetricsValue() const;
        void setMetricsValue( dclr::Metrics );
        void updateMetricsValue( dclr::Metrics );

        Edge& getEdge() const;
        EdgeId getEdgeId() const;

        bool operator== ( const QKD_Link& ) const;
};

std::ostream& operator<< ( std::ostream&, const QKD_Link& );

#endif  // QKDLINK_HPP
