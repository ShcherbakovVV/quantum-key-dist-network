#include <boost/log/trivial.hpp>

#include "KeyGenerationModel.hpp"
#include "Global.hpp"
#include "Network.hpp"


// PRIVATE FUNCTIONS
double KeyGenerationModel::_get_alpha(Distance x)
{
    return -6.02*std::log(x) + 36.7;
}


double KeyGenerationModel::_get_beta(Distance x)
{
    return 3.83*0.0001*x*x + 3.74*0.01*x + 2.06;
}


double KeyGenerationModel::_get_scale(Distance x)
{
    return -2.77*0.001*x*x - 8.75*0.01*x + 62;
}
// END OF PRIVATE FUNCTIONS


// PUBLIC FUNCTIONS
KeyData KeyGenerationModel::key_data(Descriptor link) const
{
    return {PROP_TABLE.property_as<KeyAmount>(link, "key amount"),
            PROP_TABLE.property_as<KeyRate>(link, "key rate, bps")};
}


void KeyGenerationModel::generate_keys()
{
    auto time_passed = network().curr() - network().prev();
    double time_factor = 1000;

    double links = 0;
    KeyData avg {0, 0};
    for (size_t i = 0; i != PROP_TABLE.size().size_desc; ++i)
    {
        auto link = static_cast<Descriptor>(i);
        if (PROP_TABLE.is_link(link) && time_passed > 0)
        {
            ++links;

            auto kdata = key_data(link);
            avg.amount += kdata.amount;
            avg.rate += kdata.rate;

            auto distance = PROP_TABLE.property_as<double>(link, "distance");

            auto alpha = _get_alpha(distance);
            auto beta = _get_alpha(distance);
            auto scale = _get_scale(distance);
            _key_gen->reset_params(alpha, beta);

            kdata.amount += kdata.rate / QUANTUM_KEY_LENGTH_BITS;
            kdata.rate = scale*(*_key_gen)() * time_passed / time_factor;
            
            PROP_TABLE(link, "key amount") = to_string_with_precision(kdata.amount, 6);
            PROP_TABLE(link, "key rate, bps") = to_string_with_precision(kdata.rate, 6);
        }
    }
    if (links != 0 && time_passed != 0)
    {
        avg.amount = avg.amount / links;
        avg.rate = avg.rate * time_factor / (time_passed * links);

        BOOST_LOG_TRIVIAL(info) << "KeyGenerationModel: generated keys, avg. amount: " 
                                << avg.amount << ", avg. rate " << avg.rate << " bps";
    }
}


bool KeyGenerationModel::utilize_keys(const Path& path)
{
    if (!path)
    {
        BOOST_LOG_TRIVIAL(warning) << "KeyGenerationModel: insufficient keys";
        return false;
    }
    auto path_data = path.path_data();
    for (size_t i = 0; i != path_data.size(); ++i)
    {
        auto link = static_cast<Descriptor>(i);
        if (PROP_TABLE.is_link(link) && key_data(link).amount < 1)
        {
            BOOST_LOG_TRIVIAL(warning) << "KeyGenerationModel: insufficient keys";
            return false;
        }
    }
    for (size_t i = 0; i != path_data.size(); ++i)
    {
        auto link = static_cast<Descriptor>(i);
        auto key_amount = key_data(link).amount - 1;
        PROP_TABLE(link, "key amount") = std::to_string(key_amount);
    }
    BOOST_LOG_TRIVIAL(info) << "KeyGenerationModel: utilized keys";
    return true;
}
// END OF PUBLIC FUNCTIONS
