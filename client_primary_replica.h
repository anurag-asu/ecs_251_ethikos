#include<iostream>
using namespace std;

class ClientPrimaryReplica {
    private:
        string replicaAddress;
    public:
        ClientPrimaryReplica(std::string);
        virtual void ConnectToPrimaryReplica();
}; 