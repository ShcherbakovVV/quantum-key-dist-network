#include "PropertyTable.hpp"


#define PROPMAP_OP_BKT  "PropertyMap::operator[]: non-existing property: "
#define _DESC_VAL_CHCK1 "PropertyTable: invalid descriptor value: "
#define _DESC_VAL_CHCK2 "PropertyTable: deleted descriptor: "
#define _PROPERTY_CHECK "PropertyTable: inexistent property: "
#define GET_ENTRY_TYPE  _DESC_VAL_CHCK1
#define OP_BRACKETS     "PropertyTable::operator(): deleted property: "


// PropertyMap
// PUBLIC FUNCTIONS
std::string PropertyMap::operator[](const std::string& property) const
{
    std::string ret;
    try {
        ret = _map.at(property);
    } catch (std::out_of_range& oor) {
        throw std::out_of_range {PROPMAP_OP_BKT + property};
    }
    return ret;
}
// END OF PUBLIC FUNCTIONS


// PropertyTable
// PRIVATE FUNCTIONS
void PropertyTable::_desc_value_check(Descriptor desc) const
{
    if (desc >= _desc_table.size())
        throw std::out_of_range {_DESC_VAL_CHCK1 + std::to_string(desc)};
        
    if (_desc_table[desc] == DescType::NOT_A_DESC)
        throw std::out_of_range {_DESC_VAL_CHCK2 + std::to_string(desc)};
}


void PropertyTable::_property_check(const std::string& property) const
{
    if (!_prop_table.contains(property))
        throw std::out_of_range {_PROPERTY_CHECK + property};
}
// END OF PRIVATE FUNCTIONS


// PUBLIC FUNCTIONS
bool PropertyTable::is_node(Descriptor desc) const
{
    return entry_type(desc) == DescType::NODE;
}


bool PropertyTable::is_link(Descriptor desc) const
{
    return entry_type(desc) == DescType::LINK;
}


DescType PropertyTable::entry_type(Descriptor desc) const
{
    DescType ret;
    try {
        ret = _desc_table.at(desc);
    } catch (std::out_of_range& oor) {
        throw std::out_of_range {GET_ENTRY_TYPE};
    }
    return ret;
}


PropTableSize PropertyTable::size() const
{
    auto count_deleted = [](const auto& cont, const auto& del) -> size_t
    {
        size_t size = cont.size();
        for (auto iter = cont.begin(); 
             (iter = std::find(iter, cont.end(), del)) != cont.end(); 
             ++iter)
        {
            --size;
        }
        return size;  // one iteration is guaranteed
    };
    size_t dtable_size = count_deleted(_desc_table, DescType::NOT_A_DESC);
    size_t ptable_size = _prop_table.size();

    return {dtable_size, ptable_size};
}


PropTableSize PropertyTable::size_with_deleted() const
{
    size_t dtable_size = _desc_table.size();
    size_t ptable_size = _prop_table.size();

    return {dtable_size, ptable_size};
}


Descriptor PropertyTable::add_entry(DescType dtype)
{
    for (auto& [_, value_list] : _prop_table)
        value_list.push_back("");  // no property value
    
    Descriptor desc = DescriptorCounter::add();
    _desc_table.push_back(dtype);
    
    assert((_desc_table.size() == desc + 1u));
    return desc;
}


void PropertyTable::delete_entry(Descriptor desc)
{
    _desc_value_check(desc);
    
    _desc_table[desc] = DescType::NOT_A_DESC;
}


void PropertyTable::add_property(const std::string& property)
{
    if (_prop_table.contains(property))
        return;
    
    _prop_table.insert({property, 
                        std::vector<std::string> (_desc_table.size(), "")});
}


void PropertyTable::clear()
{
    _prop_table = {{}, {}};
    _desc_table = {};
}


PropertyMap PropertyTable::operator[](Descriptor desc) const
{
    _desc_value_check(desc);

    std::map<std::string, std::string> property_map {};
    for (const auto& [property, value_list] : _prop_table)
    {      
        const auto& property_value = value_list[desc];
        property_map.insert({property, property_value});
    }
    return {property_map};
}


DescType PropertyTable::operator()(Descriptor desc) const
{
    return entry_type(desc);
}


std::string& 
PropertyTable::operator()(Descriptor desc, const std::string& property)  
{
    _desc_value_check(desc);
    _property_check(property); 
    
    return _prop_table.at(property)[desc]; 
}


// clang++: infinite recursion
const std::string& 
PropertyTable::operator()(Descriptor desc, const std::string& property)
const
{
    _desc_value_check(desc);
    _property_check(property); 
    
    return _prop_table.at(property)[desc];
}

// END OF PUBLIC FUNCTIONS
