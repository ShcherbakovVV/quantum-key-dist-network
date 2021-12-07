#ifndef QKDREQUESTGEN_HPP
#define QKDREQUESTGEN_HPP

#include <chrono>
#include <random>

#include <boost/log/trivial.hpp>

#include "lib/uintrng.hpp"
#include "lib/timemoment.hpp"

#include "edge.hpp"
#include "vertex.hpp"
#include "qkdtopology.hpp"

using std::chrono_literals::operator""ms;

template <typename NetworkModel>
class QKD_RequestGen
{
public:

    template <typename NetPtr, typename ModPtr>
        friend void assignParent( NetPtr, ModPtr );

    using Clock     = typename NetworkModel::Clock;
    using Duration  = typename NetworkModel::Duration;
    using Metrics   = typename NetworkModel::Metrics;

    using QKD_Node = typename NetworkModel::QKD_Node;

    using NodeId = typename NetworkModel::NodeId;

    using TimePoint = typename NetworkModel::TimePoint;
    using IntRNG    = typename NetworkModel::IntRNG;
    using Engine    = typename IntRNG::Engine;
    using Distrib   = typename IntRNG::Distrib;

    using Request = typename NetworkModel::Request;

private:

    NetworkModel* mpQKD_Network = nullptr;

    IntRNG mIntRNG;
    Duration mRequestLifetime;

    NodeId genRandomNodeId();  // не const из-за IntRNG

public:

    QKD_RequestGen ( Duration age );

    ~QKD_RequestGen ();

    Request genRequest();  // не const из-за IntRNG
    void setRequestLifetime( Duration );
    void updGenParams();
};

template <typename NetworkModel>
QKD_RequestGen<NetworkModel>::QKD_RequestGen ( Duration age )
:
    mRequestLifetime { age },
    mIntRNG          ( 1,
                       QKD_Node::getLastNodeId().value(),
                       Engine {},
                       Distrib { 0, NodeId::max().value() } )
{
    BOOST_LOG_TRIVIAL(trace) << "Constructed QKD_RequestGen";
}

template <typename NetworkModel>
QKD_RequestGen<NetworkModel>::~QKD_RequestGen ()
{
    BOOST_LOG_TRIVIAL(trace) << "Destructed QKD_RequestGen";
}

template <typename NetworkModel>
typename QKD_RequestGen<NetworkModel>::NodeId
QKD_RequestGen<NetworkModel>::genRandomNodeId()
{
    NodeId res;
    do {
        res = mIntRNG();
        if ( !mpQKD_Network->isNodeRemoved( res )
             && !mpQKD_Network->isAuxNode( res ) )
            break;
    }
    while (true);
    return res;
}

template <typename NetworkModel>
typename QKD_RequestGen<NetworkModel>::Request
QKD_RequestGen<NetworkModel>::genRequest()
{
    NodeId start, dest;
    start = genRandomNodeId();
    do
        dest = genRandomNodeId();
    while ( start == dest );

    TimePoint now {};
    TimePoint expir = now + mRequestLifetime;
    Request req { now, expir, start, dest };
    BOOST_LOG_TRIVIAL(info) << "QKD_RequestGen: generated " << req;
    return req; 
}

template <typename NetworkModel>
void QKD_RequestGen<NetworkModel>::setRequestLifetime( Duration age )
{
    mRequestLifetime = age;
    BOOST_LOG_TRIVIAL(info)
        << "QKD_RequestGen: set Request Lifetime to " << age;
}

template <typename NetworkModel>
void QKD_RequestGen<NetworkModel>::updGenParams()
{
    mIntRNG.max_out( QKD_Node::getLastNodeId().value() );
    BOOST_LOG_TRIVIAL(trace) << "QKD_RequestGen: RNG params has been updated";
}

#endif  // QKDREQUESTGEN_HPP
