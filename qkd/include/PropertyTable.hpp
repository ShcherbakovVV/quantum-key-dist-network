#ifndef PROPERTY_TABLE_HPP
#define PROPERTY_TABLE_HPP

#include <cassert>
#include <concepts>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "Descriptor.hpp"
#include "Global.hpp"


/* TODO:
 *  - Tagged enum PropertyValue for encapsulating different types
 *  - Marking deleted properties instead of erasing them
 *  - Remove public access from PropertyTable
 */

class Network;


// wrapper class, used for convenience
class PropertyMap
{
private:
    std::map<std::string, std::string> _map;
    
public:
    PropertyMap (const std::map<std::string, std::string>& prop_map)
    :
        _map {prop_map}
    {}

    std::map<std::string, std::string>&       operator->()       { return _map; }
    const std::map<std::string, std::string>& operator->() const { return _map; }

    std::string operator[](const std::string& property) const;
};


struct PropTableSize
{
    size_t size_desc;
    size_t size_prop;
};


class PropertyTable
{
    SINGLETON(PropertyTable)

private:
    friend class Network;

    std::map<std::string, std::vector<std::string>> _prop_table;
    std::vector<DescType> _desc_table;
    
    void _desc_value_check(Descriptor) const;
    void _property_check(const std::string&) const;
    
public:
    bool is_node(Descriptor) const;
    bool is_link(Descriptor) const;

    DescType entry_type(Descriptor) const;
    DescType operator()(Descriptor) const;

    PropTableSize size() const;
    PropTableSize size_with_deleted() const;

    Descriptor add_entry(DescType);
    void delete_entry(Descriptor);

    void add_property(const std::string&);

    void clear();
    
    PropertyMap operator[](Descriptor) const;
    
    std::string&       operator()(Descriptor, const std::string&);
    const std::string& operator()(Descriptor, const std::string&) const;

    template<std::integral I>
    I property_as(Descriptor, const std::string&) const;

    template<std::floating_point F>
    F property_as(Descriptor, const std::string&) const;
};


#undef PROP_TABLE
#define PROP_TABLE PropertyTable::instance()


template<std::integral I>
I PropertyTable::property_as(Descriptor desc, const std::string& property) const
{
    const auto& prop_val = operator()(desc, property);
    if (prop_val == "")
        return static_cast<I>(0);
    
    return static_cast<I>(std::stoll(prop_val));
}


template<std::floating_point F>
F PropertyTable::property_as(Descriptor desc, const std::string& property) const
{
    const auto& prop_val = operator()(desc, property);
    if (prop_val == "")
        return static_cast<F>(0);
    
    return static_cast<F>(std::stold(prop_val));
}


#endif  // PROPERTY_TABLE_HPP
