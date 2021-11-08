#ifndef QKDKEYGENMODEL_HPP
#define QKDKEYGENMODEL_HPP

#include <boost/log/trivial.hpp>

#include "qkdlink.hpp"
#include "qkdnode.hpp"

template <typename NetworkModel>
class QKD_KeyGenModel
{
public:

    template <typename NetPtr, typename ModPtr>
        friend void assignParent( NetPtr, ModPtr );

    using Metrics = typename NetworkModel::Metrics;

    using QKD_Link = typename NetworkModel::QKD_Link;

    using NodeId = typename NetworkModel::NodeId;
    using LinkId = typename NetworkModel::LinkId;

private:

    NetworkModel* mpQKD_Network = nullptr;

public:

    QKD_KeyGenModel ();

    ~QKD_KeyGenModel ();

    void updateLinkMetrics( LinkId, Metrics );
};

template <typename NetworkModel>
QKD_KeyGenModel<NetworkModel>::QKD_KeyGenModel ()
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_KeyGenModel";
}

template <typename NetworkModel>
QKD_KeyGenModel<NetworkModel>::~QKD_KeyGenModel ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_KeyGenModel";
}

template <typename NetworkModel>
void QKD_KeyGenModel<NetworkModel>::updateLinkMetrics( LinkId l, Metrics m )
{
    QKD_Link& link = mpQKD_Network->getLinkById( l );
    link.setMetricsValue( link.getMetricsValue() + m );
}

#endif  //  QKDKEYGENMODEL_HPP
