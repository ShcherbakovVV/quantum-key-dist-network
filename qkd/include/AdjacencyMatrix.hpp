#ifndef ADJACENCY_MATRIX_HPP
#define ADJACENCY_MATRIX_HPP

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include "Descriptor.hpp"
#include "Graph.hpp"

/* TODO:
 *  - Add node and link iteration methods
 */

class PropertyTable;

// Adjacency matrix - unordered graph representation
// Lower triangular matrix consists of descriptors to adjacent elements
// Links regarded as vertices, each link must have 2 adjacent nodes
// 
// N - NOT_A_DESC
// 
//    n0 -- n1 -- n3
//       l2    l4
//
//     | n0 n1 l2 n3 n4
// ----|----------------
//  n0 | N  .  .  .  .
//  n1 | l2 N  .  .  .
//  l2 | n1 n0 N  .  .
//  n3 | N  l4 N  N  .
//  l4 | N  n3 N  n1 N

class AdjacencyMatrix : public Graph
{
private:
    std::vector<Descriptor> _matrix {};
    size_t _size = 0;  // matrix dimension (MxM)
    
public:
    std::pair<size_t, size_t> _index_to_dim(size_t) const;
    size_t _dim_to_index(size_t, size_t) const;
    
    void       _check_desc_value(Descriptor) const;
    Descriptor _add();
    DescType   _desc_type(Descriptor desc) const;
    
public:    
    bool is_node(Descriptor) const override final;
    bool is_link(Descriptor) const override final;
    
    Descriptor add_node() override final;
    Descriptor add_link(Descriptor, Descriptor) override final;
    
    std::vector<Descriptor> adjacent(Descriptor) const override final;
    std::vector<Descriptor> operator[](Descriptor) const override final;
    
    Descriptor between(Descriptor, Descriptor) const override final;
    Descriptor operator()(Descriptor, Descriptor) const override final;
    
    //repack() -- remove all deleted vertices to reduce matrix size
};

#endif  // ADJACENCY_MATRIX_HPP
