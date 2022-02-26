#include<iostream>
using namespace std;

class ClientReplica {
    private:
        string replicaAddress;
    public:
        ClientReplica(string address);
        string GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
        bool CommitOrAbort(const std::string& action, int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
};


