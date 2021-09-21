#ifndef QKDPATHFINDER_HPP
#define QKDPATHFINDER_HPP

#include "common.hpp"

class Path
{
    private:

        std::vector< std::shared_ptr< QKD_Link > > maPathEdgesList;

        Path () = delete;
        Path ( QKD_Node& st, QKD_Node& ds );

    public:

        std::shared_ptr< QKD_Node > start;
        std::shared_ptr< QKD_Node > dest;

};

class DijkstraShortestPaths
{
    private:

        
    public:
    
        const Path& operator() ( QKD_Node& st, QKD_Node& ds );
};

template < typename Algorithm = DijkstraShortestPaths >
class QKD_Pathfinder
{
    private:
    
        Algorithm& mAlgorithm;

    public:

        
           
};

#endif  // QKDPATHFINDER_HPP
