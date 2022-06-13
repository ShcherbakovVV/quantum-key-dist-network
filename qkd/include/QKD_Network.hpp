#ifndef QKD_NETWORK_HPP
#define QKD_NETWORK_HPP

#include <array>
#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include <boost/log/trivial.hpp>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "KeyGenerationModel.hpp"
#include "Module.hpp"
#include "Network.hpp"
#include "Pathfinder.hpp"
#include "QueueManager.hpp"
#include "Request.hpp"
#include "RequestGenerator.hpp"
#include "Statistics.hpp"


class QKD_Network : public Network
{
protected:
    void _check_module_type(ModuleType) const;
    std::string _to_graphviz(std::optional<Path>) const;

public:
    QKD_Network ();
    virtual ~QKD_Network() {}

    template<typename Caller, typename ArrivalDistrib, typename ServiceDistrib>
    void simulation(Caller&, ArrivalDistrib&, ServiceDistrib&, Duration, Duration);
};


// PUBLIC FUNCTIONS
template<typename Caller, typename ArrivalDistrib, typename ServiceDistrib>
void QKD_Network::simulation(Caller& caller, ArrivalDistrib& arrival, ServiceDistrib& service, 
                             Duration step, Duration sim_time)
{
    _step = step;
    for (TimePoint t = 0; t <= sim_time; t += step)
    {
        if (caller.is_stopped())
            break;

        clock_tick(step);
        STATISTICS["completion ratio"] = std::to_string(static_cast<double>(t) / sim_time);
        caller.percentage_notify();

        auto& kgen = dynamic_cast<KeyGenerationModel&>(get_module(ModuleType::KEY_GEN_MODEL));
        auto& pfind = dynamic_cast<Pathfinder&>(get_module(ModuleType::PATHFINDER));
        auto& rgen = dynamic_cast<RequestGenerator&>(get_module(ModuleType::REQ_GENERATOR));
        auto& qman = dynamic_cast<QueueManager&>(get_module(ModuleType::QUEUE_MANAGER));

        kgen.access().generate_keys();

        std::shared_ptr<Request> req;
        if (rgen.access().arrival(arrival))
        {
            req = rgen.access().get_request();
            qman.access().enqueue(req);
        }
        if (qman.access().service(service))
        {
            auto req_opt = qman.access().dequeue();
            if (!req_opt)
                continue;

            const auto& req = req_opt.value();
            auto path = pfind.access().get_path(req->start_node, req->dest_node);

            if (kgen.access().utilize_keys(path))
            {
                auto serviced = STATISTICS.attribute_as<unsigned>("serviced requests");
                STATISTICS["serviced requests"] = std::to_string(serviced + 1);
            }
            auto overall = STATISTICS.attribute_as<unsigned>("overall requests");
            STATISTICS["overall requests"] = std::to_string(overall + 1);
        }
    }
    if (caller.is_stopped())
        caller.simulation_stop_notify();

    if (!caller.is_stopped())
        caller.simulation_end_notify();

    auto serviced = STATISTICS.attribute_as<double>("serviced requests");
    auto overall = STATISTICS.attribute_as<double>("overall requests");
    auto serv_all = serviced / overall;
    STATISTICS["serviced/overall request ratio"] = std::to_string(serv_all);
}
// END OF PUBLIC FUNCTIONS


#endif  // QKD_NETWORK_HP