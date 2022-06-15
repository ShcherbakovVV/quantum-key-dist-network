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
void KeyGenerationModel::generate_keys()
{
    auto time_passed = network().curr() - network().prev();
    double time_factor = 1000;

    double links = 0;
    KeyAmount avg_amount = 0;
    for (size_t i = 0; i != PROP_TABLE.size().size_desc; ++i)
    {
        auto link = static_cast<Descriptor>(i);
        if (PROP_TABLE.is_link(link) && time_passed > 0)
        {
            ++links;

            auto amount = PROP_TABLE.property_as<KeyAmount>(link, "key amount");
            auto rate = PROP_TABLE.property_as<KeyRate>(link, "key rate, bps");
            avg_amount += amount;

            auto distance = PROP_TABLE.property_as<double>(link, "distance");

            auto alpha = _get_alpha(distance);
            auto beta = _get_alpha(distance);
            auto scale = _get_scale(distance);
            _key_gen->reset_params(alpha, beta);

            amount += (rate / QUANTUM_KEY_LENGTH_BITS);
            //BOOST_LOG_TRIVIAL(info) << rate;
            rate = scale * (*_key_gen)() * time_passed / time_factor;
            
            PROP_TABLE(link, "key amount") = to_string_with_precision(amount, 6);
            PROP_TABLE(link, "key rate, bps") = to_string_with_precision(rate, 6);
        }
    }
    if (links != 0 && time_passed != 0)
    {
        avg_amount = avg_amount / links;

        BOOST_LOG_TRIVIAL(info) << "KeyGenerationModel: generated keys, avg. amount: " 
                                << avg_amount;
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
        if (PROP_TABLE.is_link(link) && PROP_TABLE.property_as<KeyAmount>(link, "key amount") < 1)
        {
            BOOST_LOG_TRIVIAL(warning) << "KeyGenerationModel: insufficient keys";
            return false;
        }
    }
    for (size_t i = 0; i != path_data.size(); ++i)
    {
        auto link = static_cast<Descriptor>(i);
        auto key_amount = PROP_TABLE.property_as<KeyAmount>(link, "key amount") - 1;
        PROP_TABLE(link, "key amount") = std::to_string(key_amount);
    }
    BOOST_LOG_TRIVIAL(info) << "KeyGenerationModel: utilized keys";
    return true;
}
// END OF PUBLIC FUNCTIONS
