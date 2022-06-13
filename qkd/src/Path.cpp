#include "Path.hpp"

#define CTOR   "Path: cannot create path from node to itself: "
#define APPEND "Path: invalid link and node order: "


// PRIVATE FUNCTIONS
bool Path::_at_first_append(Descriptor desc, DescType dtype)
{
    if (_path.size() > 2)
        return false;

    if (dtype == DescType::NODE)
    {
        _first_is_node = true;
        _last_is_node = true;
    }
    if (dtype == DescType::LINK)
    {
        _first_is_node = false;
        _last_is_node = false;
    }
    _path.insert(_path.begin() + 1, desc);

    return true;
}
// END PRIVATE FUNCTIONS


// PUBLIC FUNCTIONS
Path::Path (Descriptor start, Descriptor dest)
{
    if (start == dest)
        throw std::logic_error {CTOR + std::to_string(start)};

    _path = {start, dest};
}


bool Path::in_path(Descriptor desc) const
{
    if (std::find(_path.begin(), _path.end(), desc) != _path.end())
        return true;

    return false;
}


void Path::append_node_front(Descriptor node)
{
    if (_at_first_append(node, DescType::NODE))
        return;

    if (_first_is_node)
        throw std::logic_error {APPEND + std::to_string(node)};
        
    _path.insert(_path.begin() + 1, node);
    _first_is_node = true;
}


void Path::append_link_front(Descriptor link)
{
    if (_at_first_append(link, DescType::LINK))
        return;

    if (!_first_is_node)
        throw std::logic_error {APPEND + std::to_string(link)};
        
    _path.insert(_path.begin() + 1, link);
    _first_is_node = false;
}


void Path::append_node_back(Descriptor node)
{
    if (_at_first_append(node, DescType::NODE))
        return;

    if (_last_is_node)
        throw std::logic_error {APPEND + std::to_string(node)};
        
    _path.insert(_path.end() - 1, node);
    _last_is_node = true;
}


void Path::append_link_back(Descriptor link)
{
    if (_at_first_append(link, DescType::LINK))
        return;

    if (!_last_is_node)
        throw std::logic_error {APPEND + std::to_string(link)};
        
    _path.insert(_path.end() - 1, link);
    _last_is_node = false;
}


std::vector<Descriptor> Path::path_data() const
{
    return _path;
}


Path::operator bool() const
{
    if (_path.empty() || _last_is_node || _first_is_node)
        return false;

    return true;
}
// END OF PUBLIC FUNCTIONS


std::ostream& operator<<(std::ostream& os, const Path& path)
{
    for (size_t i = 0; const auto& elem : path.path_data())
    {
        if (i % 2 == 0) os << "Node ";
        else            os << "Link ";

        os << std::to_string(elem);
        if (i != path.path_data().size() - 1)
            os << " -> ";
            
        ++i;
    }
    return os;
}