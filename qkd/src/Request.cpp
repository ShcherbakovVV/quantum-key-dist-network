#include "Request.hpp"


std::ostream& operator<<(std::ostream& os, const Request& req)
{
    os << "Node " << req.start_node
                  << " (" << PropertyTable::instance()(req.start_node, "label") << ") to " 
       << "Node " << req.dest_node
                  << " (" << PropertyTable::instance()(req.dest_node, "label") << ")";
    return os;
}


std::ostream& operator<<(std::ostream& os, const TimedRequest& treq)
{
    os << "Node " << treq.start_node
                  << " (" << PropertyTable::instance()(treq.start_node, "label") << ") to " 
       << "Node " << treq.dest_node
                  << " (" << PropertyTable::instance()(treq.dest_node, "label") << ")"
       << ", gen. " << treq.gen_time // time_point_to_str(treq.gen_time)
       << ", exp. " << treq.exp_time; // time_point_to_str(treq.exp_time);
    return os;
}


std::ostream& operator<<(std::ostream& os, const PriorityRequest& preq)
{
    os << "Node " << preq.start_node
                  << " (" << PropertyTable::instance()(preq.start_node, "label") << ") to " 
       << "Node " << preq.dest_node
                  << " (" << PropertyTable::instance()(preq.dest_node, "label") << ")"
       << ", priority level " << preq.priority;
    return os;
}


std::ostream& operator<<(std::ostream& os, const TimedPriorityRequest& tpreq)
{
    os << "Node " << tpreq.start_node
                  << " (" << PropertyTable::instance()(tpreq.start_node, "label") << ") to "
       << "Node " << tpreq.dest_node
                  << " (" << PropertyTable::instance()(tpreq.dest_node, "label") << ")"
       << ", gen. time " << tpreq.gen_time // time_point_to_str(tpreq.gen_time)
       << ", exp. time " << tpreq.exp_time // time_point_to_str(tpreq.exp_time)
       << ", priority level " << tpreq.priority;
    return os;
}