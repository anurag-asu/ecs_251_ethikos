#include<iostream>
using namespace std;

class ClientReplica {
    private:
        string replicaAddress;
    public:
        ClientReplica(string address);
        string GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
        bool CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
};


