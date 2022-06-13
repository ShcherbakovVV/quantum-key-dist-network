#include "Descriptor.hpp"


Descriptor DescriptorCounter::get()
{
    return (_last_free_desc == 0) ? 0 : _last_free_desc - 1;
}


Descriptor DescriptorCounter::add()
{
    if ( _saturated)
        throw std::overflow_error {"DescriptorCounter saturated"};
        
    if (_last_free_desc == DESC_VALUE_MAX)
        _saturated = true;
        
    return _last_free_desc++;
}   


void DescriptorCounter::reset()
{
    _last_free_desc = 0;
    _saturated = false;
}   


bool operator==(const Descriptor desc, const DescType dtype)
{
    return static_cast<DescType>(desc) == dtype;
}


bool operator==(const DescType dtype, const Descriptor desc)
{
    return static_cast<DescType>(desc) == dtype;
}
