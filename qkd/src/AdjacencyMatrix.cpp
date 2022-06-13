#include "AdjacencyMatrix.hpp"

#define _CHECK_D_VAL "AdjacencyMatrix: node out of range: "
#define _ADD         "AdjacencyMatrix: max descriptors number reached"
#define ADD_LINK1    "AdjacencyMatrix::add_link(): one of descriptors isn't a node: "
#define ADD_LINK2    "AdjacencyMatrix::add_link(): cannot connect node with itself: "
#define OP_SQ_BRKTS  "AdjacencyMatrix::operator[]: object is not connected to any other: "
#define OP_BRACKETS  "AdjacencyMatrix::operator(): nodes are not connected: "


// PRIVATE FUNCTIONS
std::pair<size_t, size_t> AdjacencyMatrix::_index_to_dim(size_t index) const
{
    size_t row = 0;
    size_t elems = 0;
    for (; elems < index + 1; ++row, elems += row) ;

    --row;
    size_t col = row - (elems - (index + 1));
    
    return {row, col};
}


size_t AdjacencyMatrix::_dim_to_index(size_t dim1, size_t dim2) const
{
    size_t greater = dim1;
    size_t lesser = dim2;
    if (greater < lesser)  // matrix is lower triangular
        std::swap(greater, lesser);
    
    if (dim1 == 0 && dim2 == 0)
        return 0;
    
    size_t upper_elems = 0;  // count of virtual elements at upper matrix half
    for (size_t i = 1; i != greater + 1; ++i) 
        upper_elems += (_size - i);
        
    return _size*greater + lesser - upper_elems;
}


void AdjacencyMatrix::_check_desc_value(Descriptor value) const
{
    if (value >= _size)
        throw std::out_of_range {_CHECK_D_VAL + std::to_string(value)};
}


Descriptor AdjacencyMatrix::_add()
{
    if (_size >= DESC_VALUE_MAX)
        throw std::overflow_error {_ADD};
        
    ++_size;
    _matrix.resize(_matrix.size() + _size, NOT_A_DESC);
    
    assert(_size == DescriptorCounter::get() + 1u);
    return static_cast<Descriptor>(_size-1);
}

DescType AdjacencyMatrix::_desc_type(Descriptor desc) const
{
    _check_desc_value(desc);
    
    return static_cast<DescType>(_matrix[_dim_to_index(desc, desc)]);
}
// END OF PRIVATE FUNCTIONS


// PUBLIC FUNCTIONS
bool AdjacencyMatrix::is_node(Descriptor desc) const
{
    _check_desc_value(desc);
    
    return _desc_type(desc) == DescType::NODE;
}


bool AdjacencyMatrix::is_link(Descriptor desc) const
{
    _check_desc_value(desc);
    
    return _desc_type(desc) == DescType::LINK;
}


Descriptor AdjacencyMatrix::add_node()
{
    Descriptor node = _add();
    size_t index = _dim_to_index(node, node);
    _matrix[index] = static_cast<Descriptor>(DescType::NODE);
    return node;
}


Descriptor AdjacencyMatrix::add_link(Descriptor node1, Descriptor node2)
{
    _check_desc_value(node1);
    _check_desc_value(node2);
    
    if (!is_node(node1) || !is_node(node2))
    {
        throw std::logic_error 
            {ADD_LINK1 + std::to_string(node1) + " " + std::to_string(node2)};
    }
        
    if (node1 == node2)
        throw std::logic_error {ADD_LINK2 + std::to_string(node1)};
    
    Descriptor link = _add();
    size_t index = _dim_to_index(link, link);
    _matrix[index] = static_cast<Descriptor>(DescType::LINK);
    
    auto between_links = [this, link](Descriptor node)
    {
        try {
            std::vector<Descriptor> adj_to_node = operator[](node);
            for (const auto& l : adj_to_node)
                _matrix[_dim_to_index(link, l)] = node;
        } catch (...) {}
    };
    between_links(node1);
    between_links(node2);
    
    size_t index_link_node1 = _dim_to_index(link, node1);
    size_t index_link_node2 = _dim_to_index(link, node2);
    size_t index_node1_node2 = _dim_to_index(node1, node2);
    
    _matrix[index_link_node1] = node2;
    _matrix[index_link_node2] = node1;
    _matrix[index_node1_node2] = link;
    
    return link;
}


std::vector<Descriptor> AdjacencyMatrix::adjacent(Descriptor desc) const
{
    _check_desc_value(desc);
    
    DescType opposite_type = (is_node(desc) ? DescType::LINK : DescType::NODE);
    std::vector<Descriptor> connected {};
    for (Descriptor i = 0; i != _size; ++i)
    {
        size_t index = _dim_to_index(desc, i);
        if (i != desc 
            && _matrix[index] != NOT_A_DESC 
            && _desc_type(i) == opposite_type)
        {
            connected.push_back(i);
        }
    }
    if (connected.empty())
        throw std::out_of_range {OP_SQ_BRKTS + std::to_string(desc)};
        
    return connected;
}


Descriptor AdjacencyMatrix::between(Descriptor desc1, Descriptor desc2) const
{
    _check_desc_value(desc1);
    _check_desc_value(desc2);
    
    Descriptor between = _matrix[_dim_to_index(desc1, desc2)];
    if (between == NOT_A_DESC)
    {
        throw std::out_of_range 
            {OP_BRACKETS + std::to_string(desc1) + ", " + std::to_string(desc2)};
    }
    return between;
}


std::vector<Descriptor> AdjacencyMatrix::operator[](Descriptor desc) const
{
    return adjacent(desc);
}


Descriptor AdjacencyMatrix::operator()(Descriptor desc1, Descriptor desc2) const
{
    return between(desc1, desc2);
}
// END OF PUBLIC FUNCTIONS
