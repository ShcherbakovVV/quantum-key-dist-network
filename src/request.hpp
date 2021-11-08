#ifndef REQUEST_HPP
#define REQUEST_HPP

template <typename NetworkModel>
struct RequestT
{
    template <typename Network> friend class QKD_RequestGen;

    using NodeId = typename NetworkModel::NodeId;

    using TimePoint = typename NetworkModel::TimePoint;

    TimePoint gen_time;
    TimePoint exp_time;
    NodeId start;
    NodeId dest;

    RequestT () = delete;

private:

    RequestT ( TimePoint, TimePoint, NodeId, NodeId );
};

template <typename NetworkModel>
RequestT<NetworkModel>::RequestT ( TimePoint gen, TimePoint exp,
                                   NodeId s, NodeId d )
:
    gen_time { gen },
    exp_time { exp },
    start    { s },
    dest     { d }
{}

#endif  // REQUEST_HPP
