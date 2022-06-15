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
RequestGenerator::RequestGenerator (const std::shared_ptr<RequestBuilder>& req_builder)
:
    Module {ModuleType::REQ_GENERATOR},
    _req_builder {req_builder},
    _node_chooser {DESC_VALUE_MIN, DescriptorCounter::get()}
{
    STATISTICS.add_attribute("serviced requests", "0");
    STATISTICS.add_attribute("overall requests", "0");
    STATISTICS.add_attribute("serviced/overall request ratio", "0");
}


const std::shared_ptr<Request> RequestGenerator::get_request() const
{
    _update_rng_params();

    Descriptor node_start = _random_node();
    Descriptor node_dest;
    do {
        node_dest = _random_node();
    } while (node_start == node_dest);

    auto req = _req_builder->make_request(node_start, node_dest);

    auto overall = STATISTICS.attribute_as<unsigned>("overall requests");
    STATISTICS["overall requests"] = std::to_string(overall + 1);

    BOOST_LOG_TRIVIAL(info) << "RequestGenerator: got a request: " << *req;

    return req;
}
// END PUBLIC FUNCTIONS
