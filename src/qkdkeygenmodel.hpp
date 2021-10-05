#ifndef QKDKEYGENMODEL_HPP
#define QKDKEYGENMODEL_HPP

#include "common.hpp"

template < typename NetworkModel >
class QKD_KeyGenModel
{
    private:

        const NetworkModel& mQKD_Network;

    public:
        
        QKD_KeyGenModel () = delete;
        QKD_KeyGenModel ( NetworkModel& );

        void updateLinkMetrics( LinkId, dclr::Metrics );
};

template <typename NetworkModel>
QKD_KeyGenModel<NetworkModel>::QKD_KeyGenModel ( NetworkModel& parent )
:
    mQKD_Network { parent }
{}

template < typename NetworkModel >
void 
QKD_KeyGenModel<NetworkModel>::updateLinkMetrics( LinkId l, dclr::Metrics m )
{
    QKD_Link& link = mQKD_Network.getLinkById( l );
    link.setMetricsValue( link.getMetricsValue() + m );
}

#endif  //  QKDKEYGENMODEL_HPP
