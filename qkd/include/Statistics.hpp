#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <concepts>
#include <functional>
#include <map>
#include <ostream>
#include <string>

#include "Global.hpp"


class Statistics
{
    SINGLETON(Statistics)

private:
    std::map<std::string, std::string> _attributes;
    std::vector<std::string> _blacklist;

public:
    void add_attribute(const std::string&, const std::string&);

    void ignore_attribute(const std::string&);

    std::string& operator[](const std::string&);
    const std::string& operator[](const std::string&) const;

    std::string to_str(size_t) const;

    template<std::integral I>
    I attribute_as(const std::string&) const;

    template<std::floating_point F>
    F attribute_as(const std::string&) const;
};


#undef STATISTICS
#define STATISTICS Statistics::instance()


template<std::integral I>
I Statistics::attribute_as(const std::string& attr) const
{
    const auto& attr_val = operator[](attr);
    if (attr_val == "")
        return static_cast<I>(0);
    
    return static_cast<I>(std::stoll(attr_val));
}


template<std::floating_point F>
F Statistics::attribute_as(const std::string& attr) const
{
    const auto& attr_val = operator[](attr);
    if (attr_val == "")
        return static_cast<F>(0);
    
    return static_cast<F>(std::stold(attr_val));
}

#endif  // STATISTICS_HPP