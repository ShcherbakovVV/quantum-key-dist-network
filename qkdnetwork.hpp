#ifndef QKDNETWORK_HPP 
#define QKDNETWORK_HPP 

class QKD_Network
{
    private:

        QKD_Topology& mTopology;
        QKD_Pathfinder& mPathfinder;
        QKD_KeyGenModel& mKeyGenModel;
        QKD_RequestGen& mRequestGen;

    public:

        QKD_Network (QKD_Topology& mTopology;
                     QKD_Pathfinder& mPathfinder;
                     QKD_KeyGenModel& mKeyGenModel;
                     QKD_RequestGen& mRequestGen;
}                

#endif
