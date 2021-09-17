#ifndef QKDLINK_HPP
#define QKDLINK_HPP

#include <memory>

#include "edge.hpp"
#include "common.hpp"

class QKD_Link;

using LinkId = tools::Id< QKD_Link, dclr::IdRep >;
LinkId LINK_INVALID {-1};

class QKD_Link
{
    private:
        
        LinkId id;

        //std::shared_ptr< Edge > mpEdge; 
        Edge& mEdge;

        QKD_Link () = delete;
        QKD_Link ( Edge&, dclr::Metrics );

    public:

        static inline dclr::Id last_link_id = 0;
        dclr::Metrics mMetrics;

        Edge& getEdge();

        bool operator== ( const QKD_Link& );
};

#endif  // QKDLINK_HPP
