#ifndef PATH_HPP
#define PATH_HPP

#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Descriptor.hpp"


class Path
{
private:
    std::vector<Descriptor> _path;
    bool _last_is_node = true;
    bool _first_is_node = true;
    
    bool _at_first_append(Descriptor, DescType);

public:
    Path (Descriptor, Descriptor);
    
    bool in_path(Descriptor) const;
    
    Descriptor start() const { return _path.front(); }
    Descriptor dest()  const { return _path.back(); }

    void append_node_front(Descriptor);
    void append_link_front(Descriptor);
    
    void append_node_back(Descriptor);
    void append_link_back(Descriptor);
    
    std::vector<Descriptor> path_data() const;

    explicit operator bool() const;
};


std::ostream& operator<<(std::ostream&, const Path&);

#endif  // PATH_HPP
