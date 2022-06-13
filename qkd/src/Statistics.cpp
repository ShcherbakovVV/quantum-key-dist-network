#include "Statistics.hpp"


void Statistics::add_attribute(const std::string& attr, const std::string& val = {})
{
    if (!_attributes.contains(attr))
        _attributes.insert({attr, val});

    _attributes[attr] = val;
}


void Statistics::ignore_attribute(const std::string& attr)
{
    _blacklist.push_back(attr);
}


std::string& Statistics::operator[](const std::string& attr) 
{
    if (!_attributes.contains(attr))
        _attributes.insert({attr, ""});

    return _attributes[attr];
}


const std::string& Statistics::operator[](const std::string& attr) const
{
    return _attributes.at(attr);
}


std::string Statistics::to_str(size_t tabs) const
{
    std::string ret {};
    std::string tab (tabs, ' ');
    for (const auto& [attr, val] : _attributes)
    {
        if (std::find(_blacklist.begin(), _blacklist.end(), attr) == _blacklist.end())
            ret +=  tab + attr + ": " + val + "\n";
    }
    return ret;
}