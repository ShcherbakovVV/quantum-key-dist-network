#include "RequestGenerator.hpp"


// PRIVATE FUNCTIONS
void RequestGenerator::_update_rng_params() const
{
    _node_chooser.reset_params(DESC_VALUE_MIN, DescriptorCounter::get());
}


Descriptor RequestGenerator::_random_node() const
{
    Descriptor rand;
    DescType dtype;
    do {
        rand = _node_chooser();
        dtype = PROP_TABLE(rand);
    } while (dtype != DescType::NODE 
             || (dtype == DescType::NODE && PROP_TABLE(rand, "type") != "target"));
    
    return rand;
}
// END OF PRIVATE FUNCTIONS


// PUBLIC FUNCTIONS
const std::shared_ptr<Request> RequestGenerator::get_request() const
{
    _update_rng_params();

    Descriptor node_start = _random_node();
    Descriptor node_dest;
    do {
        node_dest = _random_node();
    } while (node_start == node_dest);

    auto req = _req_builder->make_request(node_start, node_dest);

    BOOST_LOG_TRIVIAL(info) << "RequestGenerator: got a request: " << *req;

    return req;
}
// END PUBLIC FUNCTIONS
