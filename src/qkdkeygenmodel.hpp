#ifndef QKDKEYGENMODEL_HPP
#define QKDKEYGENMODEL_HPP

#include <boost/log/trivial.hpp>

#include "common.hpp"

template < typename NetworkModel >
class QKD_KeyGenModel
{
    private:

        NetworkModel& mQKD_Network;

    public:
        
        QKD_KeyGenModel () = delete;
        QKD_KeyGenModel ( NetworkModel& );
        
        ~QKD_KeyGenModel ();

        void updateLinkMetrics( LinkId, dclr::Metrics );
};

template <typename NetworkModel>
QKD_KeyGenModel<NetworkModel>::QKD_KeyGenModel ( NetworkModel& parent )
:
    mQKD_Network { parent }
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_KeyGenModel";
}

template <typename NetworkModel>
QKD_KeyGenModel<NetworkModel>::~QKD_KeyGenModel ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_KeyGenModel";
}

template <typename NetworkModel>
void 
QKD_KeyGenModel<NetworkModel>::updateLinkMetrics( LinkId l, dclr::Metrics m )
{
    QKD_Link& link = mQKD_Network.getLinkById( l );
    link.setMetricsValue( link.getMetricsValue() + m );
}

#endif  //  QKDKEYGENMODEL_HPP
