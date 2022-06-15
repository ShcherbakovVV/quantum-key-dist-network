#ifndef KEY_GENERATION_MODEL_HPP
#define KEY_GENERATION_MODEL_HPP

#include <chrono>
#include <cmath>
#include <random>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include <boost/random/beta_distribution.hpp>

#include "Descriptor.hpp"
#include "Global.hpp"
#include "Module.hpp"
#include "Path.hpp"
#include "PropertyTable.hpp"
#include "RNG.hpp"
#include "Topology.hpp"


namespace br = boost::random;


class KeyGenerationModel : public Module
{
private:    
    const std::shared_ptr<RNG<br::beta_distribution<KeyRate>>> _key_gen;

    double _get_alpha(Distance);
    double _get_beta(Distance);
    double _get_scale(Distance);
    
public:
    KeyGenerationModel (const std::shared_ptr<RNG<br::beta_distribution<KeyRate>>>& dist)
    :
        Module {ModuleType::KEY_GEN_MODEL},
        _key_gen {dist}
    {
        // move to main program
        PROP_TABLE.add_property("key amount");
        PROP_TABLE.add_property("key rate, bps");
        PROP_TABLE.add_property("distance");
    }
    
    virtual KeyGenerationModel&       access()       override { return *this; }
    virtual const KeyGenerationModel& access() const override { return *this; }

    virtual void generate_keys();
    virtual bool utilize_keys(const Path&);
};


#endif  // KEY_GENERATION_MODEL_HPP
